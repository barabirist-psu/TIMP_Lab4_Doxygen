/**
 * @file tableCipher.cpp
 * @author Ганьшин В.А.
 * @version 1.0
 * @date 17.12.2025
 * @brief Реализация класса TableCipher для шифрования табличной маршрутной перестановки
 * @details Содержит полную реализацию всех методов класса TableCipher:
 * - Конструктор с валидацией ключа
 * - Метод encrypt() для шифрования текста
 * - Метод decrypt() для расшифрования текста
 * - Метод validateKey() для проверки корректности ключа
 * @copyright Учебный проект
 * @warning Реализация поддерживает только буквы и пробелы
 */

#include "tableCipher.h"
#include <algorithm>
#include <iostream>
#include <cwctype>
#include <stdexcept>

/**
 * @brief Конструктор класса TableCipher
 * @param key Количество столбцов таблицы (ключ шифрования)
 * @throw table_cipher_error Если ключ некорректен
 * @details Выполняет валидацию ключа и инициализацию объекта.
 * Ключ должен быть положительным целым числом в диапазоне от 1 до 1000.
 * 
 * Пример использования:
 * @code
 * TableCipher cipher(5); // Создание шифра с ключом 5
 * @endcode
 */
TableCipher::TableCipher(int key) {
    validateKey(key); // Проверка корректности ключа
    numColumns = key; // Установка количества столбцов
}

/**
 * @brief Проверка корректности ключа шифрования
 * @param key Ключ для проверки
 * @throw table_cipher_error Если ключ <= 0 или > 1000
 * @details Выполняет следующие проверки:
 * - Ключ должен быть положительным числом
 * - Ключ не должен превышать максимальное значение 1000
 * 
 * Примеры ошибок:
 * @code
 * validateKey(-5);  // Выбросит исключение: "Ключ должен быть положительным числом"
 * validateKey(2000); // Выбросит исключение: "Ключ слишком большой..."
 * @endcode
 */
void TableCipher::validateKey(int key) {
    if (key <= 0) {
        throw table_cipher_error("Ключ должен быть положительным числом");
    }
    if (key > 1000) {
        throw table_cipher_error("Ключ слишком большой. Максимальное значение: 1000");
    }
}

/**
 * @brief Шифрование текста методом табличной маршрутной перестановки
 * @param text Исходный текст для шифрования
 * @return Зашифрованная строка
 * @throw table_cipher_error При некорректных входных данных
 * @details Алгоритм шифрования:
 * 1. **Проверка входных данных**: текст не должен быть пустым и содержать только буквы и пробелы
 * 2. **Создание таблицы**: вычисление количества строк и создание таблицы numRows × numColumns
 * 3. **Запись в таблицу**: заполнение таблицы по горизонтали слева направо, сверху вниз
 * 4. **Чтение из таблицы**: чтение данных сверху вниз, справа налево
 * 
 * Пример работы:
 * @code
 * Текст: "ПРИВЕТМИР", Ключ: 3
 * Таблица:
 *   П Р И
 *   В Е Т  
 *   М И Р
 * Результат: "ИТРЕРВИПМ"
 * @endcode
 */
std::wstring TableCipher::encrypt(const std::wstring& text) {
    // Проверка входного текста на пустоту
    if (text.empty()) {
        throw table_cipher_error("Пустой текст для шифрования!");
    }
    
    // Проверка символов текста на допустимость
    for (wchar_t c : text) {
        if (!std::iswalpha(c) && c != L' ') {
            throw table_cipher_error("Текст содержит недопустимые символы! Разрешены только буквы и пробелы.");
        }
    }
    
    // Проверка, что ключ не больше длины текста
    if (numColumns > static_cast<int>(text.length())) {
        throw table_cipher_error("Ключ не может быть больше длины текста");
    }
    
    // Вычисление количества строк таблицы
    int textLength = static_cast<int>(text.length());
    int numRows = (textLength + numColumns - 1) / numColumns;
    
    // Проверка на слишком большую таблицу (защита от переполнения)
    if (numRows > 10000 || numColumns > 10000) {
        throw table_cipher_error("Слишком большая таблица для шифрования");
    }
    
    // Создание таблицы для заполнения
    // Инициализация таблицы пробелами размером numRows × numColumns
    std::vector<std::vector<wchar_t>> table(numRows, std::vector<wchar_t>(numColumns, L' '));
    
    // ЗАПИСЬ: по горизонтали слева направо, сверху вниз
    int index = 0;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numColumns; col++) {
            if (index < textLength) {
                table[row][col] = text[index++];
            }
        }
    }
    
    // ЧТЕНИЕ: сверху вниз, справа налево
    std::wstring result;
    for (int col = numColumns - 1; col >= 0; col--) {
        for (int row = 0; row < numRows; row++) {
            // Добавляем только непустые ячейки
            if (table[row][col] != L' ') {
                result += table[row][col];
            }
        }
    }
    
    return result;
}

/**
 * @brief Расшифрование текста методом табличной маршрутной перестановки
 * @param cipher_text Зашифрованный текст
 * @return Расшифрованная строка
 * @throw table_cipher_error При некорректных входных данных
 * @details Алгоритм расшифрования (обратный шифрованию):
 * 1. **Проверка входных данных**: текст не должен быть пустым и содержать только буквы и пробелы
 * 2. **Создание таблицы**: вычисление количества строк и создание таблицы numRows × numColumns
 * 3. **Запись в таблицу**: заполнение таблицы по столбцам справа налево, сверху вниз
 * 4. **Чтение из таблицы**: чтение данных по строкам слева направо, сверху вниз
 * 
 * Особенности алгоритма:
 * - Учитывается неполнота последней строки таблицы
 * - Пустые ячейки заполняются пробелами и игнорируются при чтении
 * 
 * Пример работы:
 * @code
 * Зашифрованный текст: "ИТРЕРВИПМ", Ключ: 3
 * Таблица:
 *   П Р И
 *   В Е Т  
 *   М И Р
 * Результат: "ПРИВЕТМИР"
 * @endcode
 */
std::wstring TableCipher::decrypt(const std::wstring& cipher_text) {
    // Проверка зашифрованного текста на пустоту
    if (cipher_text.empty()) {
        throw table_cipher_error("Пустой текст для расшифровки!");
    }
    
    // Проверка символов зашифрованного текста на допустимость
    for (wchar_t c : cipher_text) {
        if (!std::iswalpha(c) && c != L' ') {
            throw table_cipher_error("Зашифрованный текст содержит недопустимые символы!");
        }
    }
    
    // Проверка, что ключ не больше длины зашифрованного текста
    if (numColumns > static_cast<int>(cipher_text.length())) {
        throw table_cipher_error("Ключ не может быть больше длины зашифрованного текста");
    }
    
    // Вычисление количества строк таблицы
    int cipherLength = static_cast<int>(cipher_text.length());
    int numRows = (cipherLength + numColumns - 1) / numColumns;
    
    // Вычисление количества символов в последней строке
    // Это необходимо для корректного заполнения таблицы при расшифровании
    int lastRowLength = cipherLength % numColumns;
    if (lastRowLength == 0) {
        lastRowLength = numColumns; // Если текст полностью заполняет таблицу
    }
    
    // Создание пустой таблицы
    std::vector<std::vector<wchar_t>> table(numRows, std::vector<wchar_t>(numColumns, L' '));
    
    // ЗАПИСЬ: заполняем таблицу по столбцам справа налево, сверху вниз
    int index = 0;
    for (int col = numColumns - 1; col >= 0; col--) {
        for (int row = 0; row < numRows; row++) {
            // Пропускаем ячейки в неполных столбцах последней строки
            // Это обеспечивает корректное восстановление исходного текста
            if (row == numRows - 1 && col >= lastRowLength) {
                continue; // Пропускаем пустые ячейки в последней строке
            }
            if (index < cipherLength) {
                table[row][col] = cipher_text[index++];
            }
        }
    }
    
    // ЧТЕНИЕ: по строкам слева направо, сверху вниз
    std::wstring result;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numColumns; col++) {
            // Добавляем только непустые ячейки
            if (table[row][col] != L' ') {
                result += table[row][col];
            }
        }
    }
    
    return result;
}

/**
 * @brief Вспомогательная функция для отладки - вывод таблицы в консоль
 * @param table Таблица для вывода
 * @param numRows Количество строк
 * @param numColumns Количество столбцов
 * @note Эта функция используется только для отладки и не включена в публичный интерфейс
 */
void debugPrintTable(const std::vector<std::vector<wchar_t>>& table, int numRows, int numColumns) {
    std::wcout << L"Отладочная информация - таблица:" << std::endl;
    for (int row = 0; row < numRows; row++) {
        std::wcout << L"  ";
        for (int col = 0; col < numColumns; col++) {
            std::wcout << table[row][col] << L" ";
        }
        std::wcout << std::endl;
    }
}
