# coding=utf-8
from ntpath import join
import sys
import glob
import os.path
import pathlib
import os
import shutil

config_path = sys.argv[1]
project_dir = ""
build_dir = ""

with open(config_path, 'r') as f:
    project_dir = f.readline().rstrip('\n')
    build_dir = f.readline().rstrip('\n')

source_dir = os.path.join(project_dir, "src")
target_dir = os.path.join(project_dir, "build", "sdk")
assets_dir = os.path.join(project_dir, "src", "engine", "assets")
templates_dir = os.path.join(project_dir, "templates")

print("project_dir: " + project_dir)
print("build_dir: " + build_dir)
print("source_dir: " + source_dir)
print("target_dir: " + target_dir)

def run_fast_scandir(dir, ext):    # dir: str, ext: list
    subfolders, files = [], []

    for f in os.scandir(dir):
        if f.is_dir():
            subfolders.append(f.path)
        if f.is_file():
            if os.path.splitext(f.name)[1].lower() in ext:
                files.append(f.path)


    for dir in list(subfolders):
        sf, f = run_fast_scandir(dir, ext)
        subfolders.extend(sf)
        files.extend(f)
    return subfolders, files

def copy_files(filespath, root_dir, target_dir):
    for flilepath in filespath:
        rel = os.path.relpath(flilepath, root_dir)
        print("rel " + rel)
        target_path = os.path.join(target_dir, rel)

        dir = os.path.dirname(target_path)
        if not os.path.exists(dir):
            os.makedirs(dir)

        print("copy " + flilepath + " to " + target_path)
        try:
            shutil.copyfile(flilepath, target_path)
        except IOError as e:
            print("Unable to copy file. %s" % e)

# copy include
_, header_files = run_fast_scandir(source_dir, [".h", ".hpp"])
copy_files(header_files, source_dir, os.path.join(target_dir, "include"))

# copy libs
_, lib_files = run_fast_scandir(build_dir, [".dll", ".lib"])
copy_files(lib_files, build_dir, os.path.join(target_dir, "lib"))

# copy assets
assets_target_dir = os.path.join(target_dir, "engine", "assets")
if os.path.exists(assets_target_dir):
    shutil.rmtree(assets_target_dir)
shutil.copytree(assets_dir, assets_target_dir)

# copy templates
templates_target_dir = os.path.join(target_dir, "templates")
if os.path.exists(templates_target_dir):
    shutil.rmtree(templates_target_dir)
shutil.copytree(templates_dir, templates_target_dir)