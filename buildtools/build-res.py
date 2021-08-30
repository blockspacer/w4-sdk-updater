#!/usr/bin/env python3
# -*- coding: utf8 -*-
import sys

if sys.version_info < (3, 7):
    sys.stderr.write('This script requires Python 3.7 or later\n')
    sys.exit(1)

import argparse
import base64
import hashlib
import json
import lzma
import mimetypes
import os
import pprint
import shutil
import subprocess
from pathlib import Path
from typing import Dict, Set

SCRIPT_DIR = Path(sys.argv[0]).resolve().parent
sys.path.append(str(SCRIPT_DIR))

import binio
import coloredlogs
import verboselogs
import w4lib

logger = verboselogs.VerboseLogger('build-res.py')

coloredlogs.install( level=os.getenv('W4_PY_LOGLEVEL', 'INFO'),fmt='[Res] %(message)s')

W4_NATIVE_TOOLS = Path()

def get_mime_type(path: Path):
    if path.suffix == '.w4pack':
        return "application/w4pack"

    mime_type = mimetypes.guess_type(path)
    if mime_type[0] is None:
        mime = "application/octet-stream"
    else:
        mime = mime_type[0]
    return mime


def encode_base85(data):
    return base64.b85encode(data, pad=True).decode("utf-8")


def make_data_uri(mime, data):
    return "data:" + mime + ";base85," + encode_base85(data)


def make_data_uri_from_file(fullpath: Path):
    mime = get_mime_type(fullpath)
    return make_data_uri(mime, fullpath.read_bytes())


def make_vfs(root_dir: Path, prefix: str = '', base85=False):
    logger.verbose(f'Making vfs from {root_dir}')
    vfs = {}

    for fullpath in root_dir.rglob('*'):
        if fullpath.is_file():
            key_path = fullpath.relative_to(root_dir).as_posix()
            value_path = prefix / fullpath.relative_to(root_dir)
            value_path = value_path.as_posix()

            if base85:
                vfs[key_path] = make_data_uri_from_file(fullpath)
            else:
                vfs[key_path] = value_path

    return vfs


class W4Package:
    def __init__(self):
        self.pack_data_array = bytearray()
        self.pack_data_header = []

        self.raw_data_array = bytearray()
        self.raw_data_header = []

    def add_file(self, full_filename: Path, archive_path: str, do_pack: bool):
        data = full_filename.read_bytes()
        mime_mype = get_mime_type(full_filename)

        if do_pack:
            array = self.pack_data_array
            header = self.pack_data_header
        else:
            array = self.raw_data_array
            header = self.raw_data_header

        header.append({
            'fnsz': len(archive_path),
            'fileName': archive_path,
            'mimesz': len(mime_mype),
            'mimeType': mime_mype,
            'offset': len(array),
            'size': len(data)
        })
        array.extend(data)

    def save(self, output_path: Path):
        logger.debug(f'writing {output_path}')
        header_bin_io = binio.new("""
                    1 : uint32          : magic
                    1 : uint32          : comprSize
                    1 : uint32          : decomptSize
                    1 : uint32          : NPackedFiles
                    1 : uint32          : NRawFiles
        """)

        file_descr_io = binio.new("""
                    1 : uint32          : fnsz
                 fnsz : string@utf8     : fileName
                    1 : uint32          : mimesz
               mimesz : string@utf8     : mimeType
                    1 : uint32          : offset
                    1 : uint32          : size
        """)

        compressed_data = lzma.compress(bytes(self.pack_data_array),
                                        format=lzma.FORMAT_ALONE,
                                        preset=lzma.PRESET_DEFAULT)

        header = {
            'magic': 100500,
            'comprSize': len(compressed_data),
            'decomptSize': len(self.pack_data_array),
            'NPackedFiles': len(self.pack_data_header),
            'NRawFiles': len(self.raw_data_header)
        }

        logger.spam('header\n' + pprint.pformat(header))

        with output_path.open("wb") as pack_file:
            header_bin_io.write_dict(pack_file, header)
            for f_descr in self.pack_data_header:
                file_descr_io.write_dict(pack_file, f_descr)

            for f_descr in self.raw_data_header:
                file_descr_io.write_dict(pack_file, f_descr)

            pack_file.write(compressed_data)
            pack_file.write(self.raw_data_array)


def need_pack(path: Path):
    return path.suffix != '.png'


def make_package(root_dir: Path, output: Path):
    logger.info(f'making package {output}')
    output.parent.mkdir(parents=True, exist_ok=True)
    package = W4Package()

    for fullpath in sorted(root_dir.rglob('*')):
        if fullpath.is_file():
            archive_path = fullpath.relative_to(root_dir).as_posix()

            package.add_file(fullpath, archive_path,
                             do_pack=need_pack(fullpath))

    package.save(output)

def run_asset_creator(*args):
    assert W4_NATIVE_TOOLS != Path()

    asset_creator_path = W4_NATIVE_TOOLS / 'W4AssetCreator'

    asset_creator_fullpath = shutil.which(asset_creator_path.name, path=str(asset_creator_path.parent))

    if not asset_creator_fullpath:
        logger.critical(f'asset_creator_path {asset_creator_path} does not exist')
        sys.exit(1)

    try:
        run_result = w4lib.run(
            asset_creator_fullpath,
            *args,
            capture_output=True,
            text=True
        )
    except subprocess.CalledProcessError as e:
        logger.critical('W4AssetCreator failed')
        logger.info(f'out:\n{e.stdout}')
        logger.info(f'err:\n{e.stderr}')
        sys.exit(1)

    if run_result.stdout:
        logger.info(f'{run_result.stdout}')
    if run_result.stderr:
        logger.info(f'{run_result.stderr}')

    return run_result

def process_resources_configurations(input_dir: Path, cache_dir: Path, output_dir: Path, build_type: str):
    #no resources - no configuration
    if not os.path.isdir(input_dir):
        return

    #select configuration file
    if os.path.isfile(input_dir/(f'AssetCreator.{build_type}.config')):
        config_file = input_dir/(f'AssetCreator.{build_type}.config')
    else:
        config_file = input_dir/(f'AssetCreator.config')

    #run asset creator
    if sys.platform == 'win32':
        run_asset_creator('--config', config_file, '--output-directory', output_dir, '--cache-directory', cache_dir, '--create-hardlinks')
    else:
        run_asset_creator('--config', config_file, '--output-directory', output_dir, '--cache-directory', cache_dir, '--create-hardlinks')


def write_vfs(vfs_path: Path, vfs_description: Dict, append=False):
    logger.verbose(f'{"Appending" if append else "Writing"} vfs to {vfs_path}')
    with vfs_path.open('a' if append else 'w') as vfs_file:
        vfs_file.write(f'window.W4_VFS_DEC={json.dumps(vfs_description)};')


def main():
    global W4_NATIVE_TOOLS

    parser = argparse.ArgumentParser(description=f"""
    Without --bundled:
        Process resources from 'project' to 'deploy/<project-name>'.
        Copy runtime from 'build' to 'deploy/<project-name>'.
    With    --bundled:
        Pack both resources and runtime in a single '<project-name>.html' to 'deploy'.

    '<project-name>' is derived from 'project' directory name.

    js/wasm files should exist in 'build'.

    Environment variable W4_NATIVE_TOOLS must provide path to native tools directory.
    """)

    parser.add_argument('project',   type=Path, help="project directory with 'resources/' subdir")
    parser.add_argument('tmp',       type=Path, help="temporary directory")
    parser.add_argument('build',     type=Path, help="build directory (where js/wasm files built)")
    parser.add_argument('deploy',    type=Path, help="project deploy directory")
    parser.add_argument('cache',     type=Path, help="project cache directory")
    parser.add_argument('build_type',type=str,  help="project build type")
    parser.add_argument('-b', '--bundled', action='store_true',help='place everything in a single index.html file')

    args = parser.parse_args()

    W4_NATIVE_TOOLS = os.getenv('W4_NATIVE_TOOLS')

    if not W4_NATIVE_TOOLS:
        logger.critical('Environment variable W4_NATIVE_TOOLS is not set')
        sys.exit(1)

    W4_NATIVE_TOOLS = Path(W4_NATIVE_TOOLS).resolve()

    logger.debug(f'W4_NATIVE_TOOLS {W4_NATIVE_TOOLS}')

    if not W4_NATIVE_TOOLS.exists():
        logger.critical(f'W4_NATIVE_TOOLS {W4_NATIVE_TOOLS} does not exist')
        sys.exit(1)


    project_dir: Path = args.project.resolve()
    tmp_dir: Path = args.tmp.resolve()
    build_dir: Path = args.build.resolve()
    deploy_dir: Path = args.deploy.resolve()
    cache_dir: Path = args.cache.resolve()
    build_type: str = args.build_type

    logger.debug(f'tools   : {W4_NATIVE_TOOLS}')
    logger.debug(f'project : {project_dir}')
    logger.debug(f'temp    : {tmp_dir}')
    logger.debug(f'build   : {build_dir}')
    logger.debug(f'deploy  : {deploy_dir}')
    logger.debug(f'cache   : {cache_dir}')

    if not project_dir.exists():
        logger.critical(f'error! {project_dir} does not exist')
        sys.exit(1)
    if not build_dir.exists():
        logger.critical(f'build_dir {build_dir} does not exist')
        sys.exit(1)

    tmp_dir.mkdir(parents=True, exist_ok=True)
    project_name = build_dir.name

    logger.debug(f'project_name {project_name}')

    if not args.bundled:
        output_dir = deploy_dir / project_name
        output_dir.mkdir(parents=True, exist_ok=True)

        process_resources_configurations(project_dir / 'resources', cache_dir, output_dir / 'resources', build_type)

        vfs = make_vfs(output_dir / 'resources', prefix='resources')

        write_vfs(output_dir / f'{project_name}.vfs.js', vfs)
    else:
        process_resources_configurations(project_dir / 'resources', cache_dir, tmp_dir / 'resources', build_type)

    logger.success(f"...ok")


if __name__ == '__main__':
    main()
