5.12. Написать программы для консольных процессов Server и Small, которые обмениваются сообщениями по анонимным каналам (2 шт.): 1 процесс записывает в первый канал, читает из второго канала,  2 процесс записывает во второй канал, читает из первого канала .Создать наследуемые дескрипторы канала.

Одновременно сообщение может передаваться только одним из процессов.

Процесс- Server, который выполняет следующие действия:
 Размер массива и элементы массива вводятся с консоли. Тип массива: double
Число N вводится с консоли
Запускает процесс Small. 
Передаёт размер массива, элементы массива и число N процессу Small. 
Получает массив  от процесса Small;
Выводит переданную и полученную информацию по каналам на консоль. 

Процесс- Small, который выполняет следующие действия.
Получает размер массива и массив чисел по анонимному каналу1 от процесса-сервера
Определяет квадратные корни  элементов массива  <0 
Передаёт их числа по анонимному каналу2 процессу-серверу. Элементы массива передаются поэлементно.
Выводит полученные числа на консоль.

Вывод в консоль во втором процессе работает через STDERR;