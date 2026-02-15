all:
    gcc main.c matrix2d.c -o matrix_app
clean:
    rm -f matrix_app
run:
    ./matrix_app
