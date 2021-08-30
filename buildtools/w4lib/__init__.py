# -*- coding: utf8 -*-
import os
import stat
import subprocess
import tempfile
from pathlib import Path
from shutil import copy, copy2, copystat, Error

import verboselogs

logger = verboselogs.VerboseLogger(__name__)


def run(executable, *args, **kwargs):
    executable_full = (executable,) + args

    executable_full = [str(i) for i in executable_full]

    logger.debug('Executing subprocess:\n' + ' '.join([f'"{i}"' for i in executable_full]))

    return subprocess.run(executable_full, check=True, **kwargs)


def run_script_bat(script_text: str, **kwargs):
    logger.debug('Running bat script\n' + script_text)

    with tempfile.TemporaryDirectory() as tmp_dir_name:
        tmp_dir = Path(tmp_dir_name)

        tmp_script = tmp_dir / 'tmp_script.bat'

        tmp_script.write_text(script_text)

        return run(tmp_script, **kwargs)


def run_script_bash(script_text: str, **kwargs):
    logger.debug('Running bash script\n' + script_text)

    with tempfile.TemporaryDirectory() as tmp_dir_name:
        tmp_dir = Path(tmp_dir_name)

        tmp_script = tmp_dir / 'tmp_script.sh'

        tmp_script.write_text(script_text)

        return run('bash', tmp_script, **kwargs)


def run_script_powershell(script_text: str, **kwargs):
    logger.debug('Running powershell script\n' + script_text)

    with tempfile.TemporaryDirectory() as tmp_dir_name:
        tmp_dir = Path(tmp_dir_name)

        tmp_script = tmp_dir / 'tmp_script.ps1'

        tmp_script.write_text(script_text)

        return run('powershell', tmp_script, **kwargs)


def win_set_environment_variable_user(key, value):
    if not value:
        try:
            run('REG', 'DELETE', 'HKCU\\Environment', '/V', key, '/f')
        except Exception as e:
            logger.exception(f'Failed to remove environment variable {key}:', exc_info=e)
    else:
        try:
            run('SETX', key, value)
        except Exception as e:
            logger.exception(f'Failed to write environment variable {key}:', exc_info=e)


def win_get_environment_variable(key):
    import winreg
    folder = None
    try:
        folder = winreg.OpenKey(winreg.HKEY_CURRENT_USER, 'Environment')
        value = str(winreg.QueryValueEx(folder, key)[0])
    except Exception as e:
        logger.exception(f'Failed to read environment variable {key}:', exc_info=e)
        return None
    finally:
        if folder is not None:
            folder.Close()

    return value


# Backport from Python 3.8 to Python 3.7
def _copytree(entries, src, dst, symlinks, ignore, copy_function,
              ignore_dangling_symlinks, dirs_exist_ok=False):
    if ignore is not None:
        ignored_names = ignore(os.fspath(src), [x.name for x in entries])
    else:
        ignored_names = set()

    os.makedirs(dst, exist_ok=dirs_exist_ok)
    errors = []
    use_srcentry = copy_function is copy2 or copy_function is copy

    for srcentry in entries:
        if srcentry.name in ignored_names:
            continue
        srcname = os.path.join(src, srcentry.name)
        dstname = os.path.join(dst, srcentry.name)
        srcobj = srcentry if use_srcentry else srcname
        try:
            is_symlink = srcentry.is_symlink()
            if is_symlink and os.name == 'nt':
                # Special check for directory junctions, which appear as
                # symlinks but we want to recurse.
                lstat = srcentry.stat(follow_symlinks=False)
                if lstat.st_reparse_tag == stat.IO_REPARSE_TAG_MOUNT_POINT:
                    is_symlink = False
            if is_symlink:
                linkto = os.readlink(srcname)
                if symlinks:
                    # We can't just leave it to `copy_function` because legacy
                    # code with a custom `copy_function` may rely on copytree
                    # doing the right thing.
                    os.symlink(linkto, dstname)
                    copystat(srcobj, dstname, follow_symlinks=not symlinks)
                else:
                    # ignore dangling symlink if the flag is on
                    if not os.path.exists(linkto) and ignore_dangling_symlinks:
                        continue
                    # otherwise let the copy occur. copy2 will raise an error
                    if srcentry.is_dir():
                        copytree(srcobj, dstname, symlinks, ignore,
                                 copy_function, dirs_exist_ok=dirs_exist_ok)
                    else:
                        copy_function(srcobj, dstname)
            elif srcentry.is_dir():
                copytree(srcobj, dstname, symlinks, ignore, copy_function,
                         dirs_exist_ok=dirs_exist_ok)
            else:
                # Will raise a SpecialFileError for unsupported file types
                copy_function(srcobj, dstname)
        # catch the Error from the recursive copytree so that we can
        # continue with other files
        except Error as err:
            errors.extend(err.args[0])
        except OSError as why:
            errors.append((srcname, dstname, str(why)))
    try:
        copystat(src, dst)
    except OSError as why:
        # Copying file access times may fail on Windows
        if getattr(why, 'winerror', None) is None:
            errors.append((src, dst, str(why)))
    if errors:
        raise Error(errors)
    return dst


# Backport from Python 3.8 to Python 3.7
def copytree(src, dst, symlinks=False, ignore=None, copy_function=copy2,
             ignore_dangling_symlinks=False, dirs_exist_ok=False):
    """Recursively copy a directory tree and return the destination directory.
    dirs_exist_ok dictates whether to raise an exception in case dst or any
    missing parent directory already exists.
    If exception(s) occur, an Error is raised with a list of reasons.
    If the optional symlinks flag is true, symbolic links in the
    source tree result in symbolic links in the destination tree; if
    it is false, the contents of the files pointed to by symbolic
    links are copied. If the file pointed by the symlink doesn't
    exist, an exception will be added in the list of errors raised in
    an Error exception at the end of the copy process.
    You can set the optional ignore_dangling_symlinks flag to true if you
    want to silence this exception. Notice that this has no effect on
    platforms that don't support os.symlink.
    The optional ignore argument is a callable. If given, it
    is called with the `src` parameter, which is the directory
    being visited by copytree(), and `names` which is the list of
    `src` contents, as returned by os.listdir():
        callable(src, names) -> ignored_names
    Since copytree() is called recursively, the callable will be
    called once for each directory that is copied. It returns a
    list of names relative to the `src` directory that should
    not be copied.
    The optional copy_function argument is a callable that will be used
    to copy each file. It will be called with the source path and the
    destination path as arguments. By default, copy2() is used, but any
    function that supports the same signature (like copy()) can be used.
    """
    # sys.audit("shutil.copytree", src, dst)
    with os.scandir(src) as itr:
        entries = list(itr)
    return _copytree(entries=entries, src=src, dst=dst, symlinks=symlinks,
                     ignore=ignore, copy_function=copy_function,
                     ignore_dangling_symlinks=ignore_dangling_symlinks,
                     dirs_exist_ok=dirs_exist_ok)
