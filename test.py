import os


pipe_path = "/tmp/data_x"

try:
    with open(pipe_path, "r") as pipe:
        data = pipe.read()
    print(data)
except FileNotFoundError:
    print("No pipe")
except OSError as err:
    print(f"Can't read form a piper! {err}")

