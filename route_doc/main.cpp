#include "tableCipher.h"
#include <iostream>
#include <string>
#include <limits>
#include <locale>
#include <codecvt>
#include <sstream>

/**
 * @file
 * @author Ганьшин В.А.
 * @version 1.0
 * @date 17.12.2025
 * @brief Главный модуль программы шифрования табличной маршрутной перестановки
 * @details Содержит пользовательский интерфейс для работы с шифром табличной перестановки
 */

/**
 * @brief Преобразование строки в широкую строку
 * @param str Обычная строка
 * @return Широкая строка
 */
std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

/**
 * @brief Преобразование широкой строки в обычную строку
 * @param wstr Широкая строка
 * @return Обычная строка
 */
std::string wstring_to_string(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

/**
 * @brief Отображение главного меню программы
 */
void displayMenu() {
    std::wcout << L"\n! ШИФР ТАБЛИЧНОЙ ПЕРЕСТАНОВКИ !\n";
    std::wcout << L"1. Зашифровать текст\n";
    std::wcout << L"2. Расшифровать текст\n";
    std::wcout << L"3. Показать справку\n";
    std::wcout << L"4. Выход\n";
    std::wcout << L"Выберите действие: ";
}

/**
 * @brief Получение ключа от пользователя с валидацией
 * @return Корректный ключ (количество столбцов)
 * @details Обрабатывает различные ошибки ввода: текст вместо чисел, символы, пустой ввод и т.д.
 */
int getKeyFromUser() {
    int key;
    while (true) {
        std::wcout << L"Введите ключ (количество столбцов): ";
        
        std::string input;
        std::getline(std::cin, input);
        
        // Проверка на пустой ввод
        if (input.empty()) {
            std::wcout << L"Ошибка: ключ не может быть пустым!\n";
            continue;
        }
        
        // Проверка, что ввод состоит только из цифр
        bool is_numeric = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                is_numeric = false;
                break;
            }
        }
        
        if (!is_numeric) {
            std::wcout << L"Ошибка: ключ должен быть положительным целым числом! Вы ввели: '" 
                      << string_to_wstring(input) << L"'\n";
            std::wcout << L"  Пожалуйста, введите ключ ещё раз\n";
            continue;
        }
        
        // Преобразование строки в число
        try {
            key = std::stoi(input);
        } catch (const std::exception& e) {
            std::wcout << L"Ошибка преобразования: неверный формат числа!\n";
            continue;
        }
        
        // Проверка на положительное число
        if (key <= 0) {
            std::wcout << L"Ошибка: ключ должен быть положительным числом!\n";
        } else if (key > 1000) {
            std::wcout << L"Ошибка: ключ слишком большой! Максимальное значение: 1000\n";
        } else {
            break; // Успешный ввод
        }
    }
    return key;
}

/**
 * @brief Получение текста от пользователя
 * @param prompt Приглашение для ввода
 * @return Введенный текст
 */
std::wstring getTextFromUser(const std::wstring& prompt) {
    std::wstring text;
    std::wcout << prompt;
    
    // Временная переменная для чтения строки
    std::string temp;
    std::getline(std::cin, temp);
    text = string_to_wstring(temp);
    
    return text;
}

/**
 * @brief Обработка операции шифрования или дешифрования
 * @param isEncryption Флаг операции: true - шифрование, false - дешифрование
 * @details Выполняет всю последовательность операций для шифрования/дешифрования с обработкой исключений
 */
void processCipherOperation(bool isEncryption) {
    try {
        int key = getKeyFromUser();
        std::wstring text = getTextFromUser(
            isEncryption ? 
            L"Введите текст для шифрования: " : 
            L"Введите текст для расшифрования: "
        );
        
        TableCipher cipher(key);
        
        if (isEncryption) {
            std::wstring encrypted = cipher.encrypt(text);
            std::wcout << L"ЗАШИФРОВАННЫЙ ТЕКСТ: " << encrypted << std::endl;
        } else {
            std::wstring decrypted = cipher.decrypt(text);
            std::wcout << L"РАСШИФРОВАННЫЙ ТЕКСТ: " << decrypted << std::endl;
        }
        
    } catch (const table_cipher_error& e) {
        std::wcout << L"ОШИБКА ШИФРОВАНИЯ: " << e.what() << std::endl;
        
        // Детализация типа ошибки
        std::string error_msg = e.what();
        if (error_msg.find("Пустой текст") != std::string::npos) {
            std::wcout << L"   ТИП ОШИБКИ: Пустой текст" << std::endl;
        } else if (error_msg.find("недопустимые символы") != std::string::npos) {
            std::wcout << L"   ТИП ОШИБКИ: Недопустимые символы" << std::endl;
        } else if (error_msg.find("Ключ должен быть") != std::string::npos) {
            std::wcout << L"   ТИП ОШИБКИ: Некорректный ключ" << std::endl;
        } else if (error_msg.find("Ключ не может быть больше") != std::string::npos) {
            std::wcout << L"   ТИП ОШИБКИ: Ключ слишком большой" << std::endl;
        } else if (error_msg.find("Слишком большая таблица") != std::string::npos) {
            std::wcout << L"   ТИП ОШИБКИ: Слишком большой текст" << std::endl;
        } else {
            std::wcout << L"   ТИП ОШИБКИ: Неизвестная ошибка шифрования" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::wcout << L"КРИТИЧЕСКАЯ ОШИБКА: " << e.what() << std::endl;
        std::wcout << L"   ТИП ОШИБКИ: Системная ошибка" << std::endl;
    }
}

/**
 * @brief Демонстрация работы шифра на тестовых примерах
 * @details Показывает корректную работу шифра и обработку различных ошибок
 */
void demonstrateCipher() {
    std::wcout << L"\n! ДЕМОНСТРАЦИЯ РАБОТЫ ШИФРА !" << std::endl;
    
    // Тест 1: Корректные данные
    std::wcout << L"\n ТЕСТ 1: Корректные данные" << std::endl;
    try {
        TableCipher cipher(3);
        std::wstring original = L"ПРИВЕТМИР";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        
        std::wcout << L"   Исходный текст: " << original << std::endl;
        std::wcout << L"   Ключ = 3" << std::endl;
        std::wcout << L"   Зашифрованный:  " << encrypted << std::endl;
        std::wcout << L"   Расшифрованный: " << decrypted << std::endl;
        
        if (original == decrypted) {
            std::wcout << L"   Шифр работает корректно!" << std::endl;
        }
    } catch (const table_cipher_error& e) {
        std::wcout << L"   Ошибка: " << e.what() << std::endl;
    }
    
    // Тест 2: Ошибочный ключ
    std::wcout << L"\n ТЕСТ 2: Некорректный ключ" << std::endl;
    try {
        TableCipher cipher(-5); // Отрицательный ключ
    } catch (const table_cipher_error& e) {
        std::wcout << L"   Ошибка: " << e.what() << std::endl;
    }
    
    // Тест 3: Пустой текст
    std::wcout << L"\n ТЕСТ 3: Пустой текст" << std::endl;
    try {
        TableCipher cipher(3);
        std::wstring encrypted = cipher.encrypt(L"");
    } catch (const table_cipher_error& e) {
        std::wcout << L"   Ошибка: " << e.what() << std::endl;
    }
    
    // Тест 4: Ключ больше длины текста
    std::wcout << L"\n ТЕСТ 4: Ключ больше длины текста" << std::endl;
    try {
        TableCipher cipher(10);
        std::wstring encrypted = cipher.encrypt(L"ПРИВЕТ");
    } catch (const table_cipher_error& e) {
        std::wcout << L"   Ошибка: " << e.what() << std::endl;
    }
}

/**
 * @brief Демонстрация обработки ошибок ввода
 * @details Показывает, какие типы ошибок ввода обрабатывает программа
 */
void demonstrateInputErrors() {
    std::wcout << L"\n! ДЕМОНСТРАЦИЯ ОШИБОК ВВОДА !" << std::endl;
    std::wcout << L"Программа обрабатывает следующие ошибки ввода ключа:\n";
    std::wcout << L" Текст вместо числа (например: 'abc', 'ключ')\n";
    std::wcout << L" Символы вместо числа (например: '!@#', '3.14')\n";
    std::wcout << L" Смешанный ввод (например: '12abc', '3 столбца')\n";
    std::wcout << L" Пустой ввод\n";
    std::wcout << L" Отрицательные числа\n";
    std::wcout << L" Слишком большие числа\n";
}

/**
 * @brief Отображение справки по программе
 * @details Содержит описание алгоритма, ограничения и демонстрационные примеры
 */
void showHelp() {
    std::wcout << L"\n! СПРАВКА !" << std::endl;
    std::wcout << L"Шифр табличной маршрутной перестановки:" << std::endl;
    std::wcout << L" Ключ: количество столбцов таблицы" << std::endl;
    std::wcout << L" Запись: по горизонтали слева направо, сверху вниз" << std::endl;
    std::wcout << L" Чтение: сверху вниз, справа налево" << std::endl;
    std::wcout << L" Ограничения:" << std::endl;
    std::wcout << L"  Текст должен содержать только буквы и пробелы" << std::endl;
    std::wcout << L"  Ключ должен быть положительным целым числом" << std::endl;
    std::wcout << L"  Ключ не может быть больше длины текста" << std::endl;
    std::wcout << L"  Максимальный ключ: 1000" << std::endl;
    
    demonstrateInputErrors();
    demonstrateCipher();
}

/**
 * @brief Главная функция программы
 * @return Код завершения программы
 * @details Реализует основной цикл программы с меню и обработкой пользовательского ввода
 */
int main() {
    // Установка локали для поддержки русского языка
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    std::wcin.imbue(std::locale("ru_RU.UTF-8"));
    
    int choice;
    
    std::wcout << L"ПРОГРАММА ШИФРОВАНИЯ - ТАБЛИЧНАЯ ПЕРЕСТАНОВКА" << std::endl;
    
    do {
        displayMenu();
        
        // Обработка ввода для меню
        std::string menu_input;
        std::getline(std::cin, menu_input);
        
        // Преобразование ввода меню в число
        try {
            choice = std::stoi(menu_input);
        } catch (const std::exception& e) {
            std::wcout << L"Неверный выбор! Пожалуйста, введите число от 1 до 4.\n";
            choice = 0; // Сброс выбора
            continue;
        }
        
        switch (choice) {
            case 1: // Шифрование
                processCipherOperation(true);
                break;
                    
            case 2: // Дешифрование
                processCipherOperation(false);
                break;
                    
            case 3: // Сравка
                showHelp();
                break;
                    
            case 4: // Выход
                std::wcout << L"До свидания!" << std::endl;
                break;
                    
            default:
                std::wcout << L"Неверный выбор! Пожалуйста, введите число от 1 до 4." << std::endl;
                break;
        }
        
    } while (choice != 4);
    
    return 0;
}
