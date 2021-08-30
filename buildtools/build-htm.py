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

logger = verboselogs.VerboseLogger('build-htm.py')

coloredlogs.install(level=os.getenv('W4_PY_LOGLEVEL', 'INFO'), fmt='[Htm] %(message)s')

ROOT_DIR = SCRIPT_DIR

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
    logger.info(f'vfs      : making from {root_dir}')
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
    logger.info(f'pack     : {output}')
    output.parent.mkdir(parents=True, exist_ok=True)
    package = W4Package()

    for fullpath in sorted(root_dir.rglob('*')):
        if fullpath.is_file():
            archive_path = fullpath.relative_to(root_dir).as_posix()

            package.add_file(fullpath, archive_path,
                             do_pack=need_pack(fullpath))

    package.save(output)


def configure_html(index_in,
                   index_out,
                   *,
                   title='',
                   wasm='',
                   vfs='',
                   entry='',
                   css=''):
    index_in = index_in.read_text()
    index_in = index_in.replace('{TITLE}', title)
    index_in = index_in.replace('{WASM}', wasm)
    index_in = index_in.replace('{VFS}', vfs)
    index_in = index_in.replace('{ENTRY}', entry)
    index_in = index_in.replace('{CSS}', css)

    logger.info(f'writing  : {index_out}')
    index_out.write_text(index_in)


def get_inline_js_tag(js: Path):
    return f'<script>{js.read_text()}</script>'


def write_vfs(vfs_path: Path, vfs_description: Dict, append=False):
    logger.info(f'{"vfs      : append " if append else "vfs      : create "} {vfs_path}')
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

    parser.add_argument('project',     type=Path, help="project directory with 'resources/' subdir")
    parser.add_argument('tmp',         type=Path, help="temporary directory")
    parser.add_argument('build',       type=Path, help="build directory (where js/wasm files built)")
    parser.add_argument('deploy',      type=Path, help="project deploy directory")
    parser.add_argument('index_in',    type=Path, help="html source template")
    parser.add_argument('index_out',   type=Path, help="html destination")
    parser.add_argument('css_file',    type=Path, help="css file")
    parser.add_argument('inject_file', type=Path, help='inject string to pack')

    parser.add_argument('-b', '--bundled', action='store_true', help='place everything in a single index.html file')
    args = parser.parse_args()

    W4_NATIVE_TOOLS = os.getenv('W4_NATIVE_TOOLS')

    if not W4_NATIVE_TOOLS:
        logger.critical('Environment variable W4_NATIVE_TOOLS is not set')
        sys.exit(1)

    W4_NATIVE_TOOLS = Path(W4_NATIVE_TOOLS).resolve()

    if not W4_NATIVE_TOOLS.exists():
        logger.critical(f'W4_NATIVE_TOOLS {W4_NATIVE_TOOLS} does not exist')
        sys.exit(1)

    project_dir: Path = args.project.resolve()
    tmp_dir: Path = args.tmp.resolve()
    build_dir: Path = args.build.resolve()
    deploy_dir: Path = args.deploy.resolve()
    index_in: Path = args.index_in.resolve()
    index_out: Path = args.index_out.resolve()
    css_file: Path = args.css_file.resolve()
    inject_file: Path =  args.inject_file.resolve()
    inject_str = ""

    if not project_dir.exists():
        logger.critical(f'project_dir {project_dir} does not exist')
        sys.exit(1)
    if not build_dir.exists():
        logger.critical(f'build_dir {build_dir} does not exist')
        sys.exit(1)

    tmp_dir.mkdir(parents=True, exist_ok=True)

    project_name = build_dir.name

    logger.debug(f'tools   : {W4_NATIVE_TOOLS}')
    logger.debug(f'project : {project_name}')
    logger.debug(f'dir     : {project_dir}')
    logger.debug(f'temp    : {tmp_dir}')
    logger.debug(f'build   : {build_dir}')
    logger.info(f'deploy  : {deploy_dir}')
    logger.info(f'htm in   : {index_in}')
    logger.info(f'htm out  : {index_out}')
    logger.info(f'css      : {css_file}')

    if inject_file.exists():
        inject_str = inject_file.read_text();
        logger.info(f'inject  : {inject_file}')

    css_source = css_file.read_text();
    entry_str = '\t<script> Module().then(m =>{let e = function (m, p) {m.cwrap("entry", "number", ["string"])(p);};e(m, "w4-main");m.W4L.setUpdate(m._draw);});</script>\n'

    if not args.bundled:
        output_dir = deploy_dir / project_name
        output_dir.mkdir(parents=True, exist_ok=True)

        vfs = make_vfs(output_dir / 'resources', prefix='resources')
        write_vfs(output_dir / f'{project_name}.vfs.js', vfs)
        configure_html(
            index_in,
            index_out,
            title=project_name,
            css='<style>\n' + css_file.read_text() + '</style>',
            entry=f'\t<script type="text/javascript" src="{project_name}.vfs.js"></script>\n'
                  f'\t<script type="text/javascript" src="{project_name}.js"></script>\n' + entry_str)

        logger.info(f'copying  : {build_dir} -> {output_dir}')
        w4lib.copytree(
            src=build_dir,
            dst=output_dir,
            dirs_exist_ok=True
        )
    else:

        js_output = Path(f'{build_dir / project_name}.js')
        js_with_vfs = tmp_dir / f'{project_name}_with_vfs.js'
        shutil.copy2(
            src=js_output,
            dst=js_with_vfs
        )

        make_package(tmp_dir / 'resources', tmp_dir / 'resources.w4pack')

        vfs = {"resources.w4pack": make_data_uri_from_file(tmp_dir / 'resources.w4pack')}
        write_vfs(js_with_vfs, vfs, append=True)

        htm_name = index_out.name;

        deploy_dir.mkdir(parents=True, exist_ok=True)

        unpacked_htm = Path(deploy_dir / f'unpacked-{project_name}-{htm_name}')

        configure_html(
            index_in,
            unpacked_htm,
            title=project_name,
            css='<style>\n' + css_file.read_text() + '</style>',
            entry=f'\n{get_inline_js_tag(js_with_vfs)}\n\n' + entry_str)

        logger.info('packing...')
        package = W4Package()
        package.add_file(unpacked_htm, '', do_pack=True)
        package.save(tmp_dir / 'index_packed.w4pack')

        js_preloader = tmp_dir / 'preloader_with_content.js'

        shutil.copy2(
            src=ROOT_DIR / 'preloader.js',
            dst=js_preloader
        )

        vfs = {"resources.w4pack": make_data_uri_from_file(tmp_dir / 'index_packed.w4pack')}

        write_vfs(js_preloader, vfs, append=True)

        final_htm = deploy_dir / f'{project_name}-{htm_name}'
        final_htm.write_text(
            '<html lang="en"><head>'
            '<meta name="viewport" content="width=device-width, height=device-height, initial-scale=1.0">'
            '<meta content="text/html;charset=utf-8" http-equiv="Content-Type">'
            '<meta content="utf-8" http-equiv="encoding"></head>'
            f'<style>{css_file.read_text()} </style> '
            f'</head><body>{inject_str}<div id = "w4-loader" class="w4-loader"><div></div><div></div><div></div></div>'
            f'{get_inline_js_tag(js_preloader)}' +
            '<script>Module().then(module => {module.W4L_Init()});</script>'
            '</body></html>')

        logger.success(f"...ok")


if __name__ == '__main__':
    main()
