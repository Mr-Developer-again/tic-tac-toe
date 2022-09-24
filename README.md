* A simple Tic Tac Toe game by C++ and using ncurses library



* To install ncurses library in Linux:

      - on Debian-based distros :
            #apt-get install -y libncurses5-dev libncursesw5-dev

      - on Redhat-based distros:
            #dnf install -y ncurses-devel


* To compile the source codes:

            #gcc main.cpp tic_tac_toe.cpp -lstdc++ -lncurses -std=c++11 -o <output_name>
