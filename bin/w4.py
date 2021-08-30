#!/usr/bin/env python3
# -*- coding: utf8 -*-
import argparse
import platform
import sys
import os
import shutil
from pathlib import Path

if sys.version_info < (3, 7):
    sys.stderr.write('This script requires Python 3.7 or later\n')
    sys.exit(1)

if not os.getenv('W4'):
    sys.stderr.write('W4 environment variable does not exist, run .install.cmd first\n')
    sys.exit(1)

W4ROOT = Path(os.getenv('W4'))
W4SDK  = W4ROOT / 'sdk'

if not W4ROOT.exists():
    sys.stderr.write('W4ROOT directory not exist\n')
    sys.exit(1)

if not W4SDK.exists():
    sys.stderr.write('W4 SDK directory not exist\n')
    sys.exit(1)

sys.path.append(str(W4SDK / 'buildtools'))

import coloredlogs
import verboselogs
import w4lib

VALID_BUILD_TYPES = ['Debug', 'Release', 'Shipping']

logger = verboselogs.VerboseLogger('w4.py')

coloredlogs.install(
    level=os.getenv('W4_PY_LOGLEVEL', 'INFO'),
    fmt='[W4] %(message)s'
)

def isW4Project(project_dir: Path):
    cmake_file = project_dir / 'CMakeLists.txt'
    if not cmake_file.exists() or 'include(W4User)' not in cmake_file.read_text():
        logger.error('Current directory is not a W4 project')
        sys.exit(1)

def create(args):
    project_dir: Path = args.path

    if project_dir.exists():
        logger.error(f'Path {project_dir} already exist')
        sys.exit(1)

    w4lib.copytree(
        W4SDK / 'template',
        project_dir,
        dirs_exist_ok=True
    )
    logger.success(f'Created new project at {project_dir} ')


def build(args):
    build_type: str = args.type

    project_dir = Path.cwd().resolve()
    isW4Project(project_dir)



    def _build_cmake(bt: str):
        logger.info(f'Building {bt}')
        build_dir = project_dir / f'.cmake/{bt}'

        if not build_dir.exists():
            w4lib.run(
                'cmake',
                '-B', build_dir,
                '-S', project_dir,
                '-GNinja',
                f'-DCMAKE_BUILD_TYPE={bt}'
            )
        w4lib.run('cmake','--build', build_dir)
        logger.success(f'Built {bt}')

    if build_type == 'All':
        for t in VALID_BUILD_TYPES:
            _build_cmake(t)
    else:
        _build_cmake(build_type)

def run(args):
    build_type: str = args.type

    project_dir = Path.cwd().resolve()
    isW4Project(project_dir)
    deploy_dir = project_dir / f'_deploy'
    logger.info('Running standard python http.server at http://localhost:8080')

    server_args = (
        sys.executable,
        '-m', 'http.server',
        '-d', deploy_dir,
        '8080'
    )
    w4lib.run('start', *server_args, shell=True)

def update(param):
    logger.info('Update W4SDK')


def main():
    parser = argparse.ArgumentParser(description='W4 Kit command line interface')
    subparsers = parser.add_subparsers()

    parser_create = subparsers.add_parser('create', help='Create new project')
    parser_create.add_argument('path',help='directory where to create new project',type=Path)
    parser_create.set_defaults(func=create)

    parser_build = subparsers.add_parser('build',help='Build project from current directory')
    parser_build.add_argument('type',help='type of build to create',choices=VALID_BUILD_TYPES + ['All'])
    parser_build.set_defaults(func=build)

    parser_run = subparsers.add_parser('run',help='Run project from current directory')
    parser_run.add_argument('type',help='type of build to run',choices=VALID_BUILD_TYPES + ['All'])
    parser_run.set_defaults(func=run)

    parser_update = subparsers.add_parser('update',help='Update W4 SDK')
    parser_update.set_defaults(func=update)


    args = parser.parse_args()

    logger.debug(f'args {args}')

    if not hasattr(args, 'func'):
        parser.print_help()
        sys.exit(1)

    args.func(args)


if __name__ == '__main__':
    main()
