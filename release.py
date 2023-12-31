import os
import subprocess
from helpers import ANSIColors

FILE_NAME = "opengl.exe"
BUILD_FOLDER = "build"
ARTIFACT_PATH = f"{BUILD_FOLDER}/{FILE_NAME}"

# Short commit hash
commit_hash = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"]).strip().decode("utf-8")

subprocess.call("python compiler.py --build --release")

if not os.path.exists(ARTIFACT_PATH):
    print(f"\n{ANSIColors.RED}Build failed!{ANSIColors.ENDC}")
    exit(1)

print(f"\n{ANSIColors.GREEN}Build complete!\n{ANSIColors.ENDC}{ANSIColors.GRAY}{FILE_NAME}{ANSIColors.ENDC}")

os.system(f'gh release create {commit_hash} {ARTIFACT_PATH} --title "Release {commit_hash}"')
