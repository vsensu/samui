# coding=utf-8

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: samui.py <cmd> <output>")
    else:
        option = sys.argv[1]
        if option == 'new':
            project_dir = sys.argv[2]
            import shutil
            import os
            sdk_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../')
            templates_dir = os.path.join(sdk_dir, 'templates/2d')
            shutil.copytree(templates_dir, project_dir)
            shutil.copytree(os.path.join(sdk_dir, 'scripts'), project_dir)