# Команды

---

## Базовые команды

### Работа с регистрами

1. `mov` **regsource,destreg**
    - Замена данных регистров
2. `reg` **regsource,data**
    - Установка данных регистров

### Работа со стеком

1. `push` **valr,...**
    - Добавление в стек значения
2. `pop` **(reg,...)**
    - Удаление и копирование элемента стека
3. `peek` **reg**
    - Копирование элемента стека

### Работа с кучей

1. `alс` **sizem,sizet** -> sizem
    - Выделение динамической памяти, ячейка с размером `sizet` и кол-во ячеек `size_m`
2. `fr` **addr,sizem** -> addr
    - Освобождение динамической памяти по заданному адресу и размеру
3. `gt` **addr,inx** -> result
    - Получение байтов по адресу и индексу

### Булевы операции

1. `eql` **valr1,(valr2)** -> result
    - Выясняет, равны ли операнды
2. `less` **valr1,(valr2)** -> result
    - Выясняет, меньше ли первый операнд, чем второй
3. `more` **valr1,(valr2)** -> result
    - Выясняет, больше ли первый операнд, чем второй
4. `neql` **valr1,(valr2)** -> result
    - Выясняет, не равны ли операнды

### Возможность перескакивать на метки

1. `jmp` **lname**
    - Перескакивает на определенную метку
2. `jep` **lname**
    - Перескакивает на определенную метку, если последний элемент стека равен 1
3. `jnp` **lname**
    - Перескакивает на определенную метку, если последний элемент стека равен 0

### Арифметические операции

1. `sum` **valr1,(valr2,...)** -> result
    - Получает сумму операндов
2. `sub` **valr1,valr2** -> result
    - Получает разность операндов
3. `div` **valr1,valr2** -> result
    - Получает частное операндов
4. `mlt` **valr1,(valr2,...)** -> result
    - Получает произведение операндов
5. `pwr` **valr1,valr2** -> result
    - Получает результат возведения первого операнда в степень второго

## Продвинутые команды

### Работа с адресами памяти

1. `ptr` **name** -> addr
    - Получает адрес переменной в куче
2. `name` **ptr** -> value
    - Получает значение по адресу в куче

### Управление типами

#### Преобразование численной переменной в буквенную и обратно

<!-- 1. `chr` **valr** -> char
    - Преобразует численный код в буквенное представление
2. `ord` **valr** -> byte
    - Преобразует букву в численное представление -->

### Возможность реализации массивов

- Использование `alloc` и `get`

### Возможность записи и чтения файлов

1. `istr` **path** -> addr,size
    - Открывает поток чтения из файла
2. `ostr` **path** -> addr
    - Открывает поток записи в файл

- Для доступа использовать арифметику указателей

### Связь с другими файлами

1. `${file}`
    - Подключает другой файл кода

### Вызов системных функций

1. `syscall` **name (args)**
    - Вызывает системную функцию

### Прямая связь с консолью

1. `clean`
    - Очищает консоль
2. `cursor` **x,y**
    - Устанавливает курсор на определенное положение
3. `color` **r,g,b**
    - Устанавливает цвет курсора
4. `setch` **ch/cd**
    - Устанавливает букву на курсоре (на вход буква/код буквы)
