sudo apt install libgtk-3-dev libcairo2-dev

Компиляция 
gcc example.c -o example `pkg-config --cflags --libs gtk+-3.0` 

Программа вернет два графика(скорость и координата) и два текстовых файла с таблицами со значениями скорости и координаты. 
