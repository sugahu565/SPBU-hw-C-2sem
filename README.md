Репо для домашек по C  
Автор - Тупикова Екатерина, Б42  
Сборка:  
Из корня репо  
'''sh
cmake . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
'''  
Внутри build  
'''sh
make help  
make (выбрали, что собрать)  
'''
Если запуск тестов, то из build запускать  
'''sh
ctest  
'''

