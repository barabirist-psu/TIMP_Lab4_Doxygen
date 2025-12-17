#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <locale>
#include <codecvt>

/**
 * @file
 * @author Ганьшин В.А.
 * @version 1.0
 * @date 17.12.2025
 * @brief Заголовочный файл для модуля шифрования табличной маршрутной перестановки
 * @warning Это документирование программы шифрования методом маршрутной перестановки
 */

/**
 * @brief Класс для исключений шифрования табличной перестановки
 * @details Наследуется от std::exception для интеграции со стандартной системой исключений
 */
class table_cipher_error : public std::exception {
private:
    std::string message; ///< Сообщение об ошибке
public:
    /**
     * @brief Конструктор исключения
     * @param msg Сообщение об ошибке
     */
    explicit table_cipher_error(const std::string& msg) : message(msg) {}
    
    /**
     * @brief Получить сообщение об ошибке
     * @return Указатель на строку с сообщением об ошибке
     */
    const char* what() const noexcept override {
        return message.c_str();
    }
};

/**
 * @brief Шифр табличной маршрутной перестановки
 * @details Реализует шифрование методом табличной перестановки с заданным количеством столбцов.
 * Запись: по горизонтали слева направо, сверху вниз.
 * Чтение: сверху вниз, справа налево.
 * @warning Поддерживает только буквы и пробелы
 */
class TableCipher {
private:
    int numColumns; ///< Количество столбцов таблицы (ключ шифрования)

public:
    /**
     * @brief Конструктор с установкой ключа
     * @param key Количество столбцов таблицы
     * @throw table_cipher_error Если ключ некорректен
     */
    TableCipher(int key);
    
    /**
     * @brief Метод шифрования текста
     * @param text Исходный текст для шифрования
     * @return Зашифрованная строка
     * @throw table_cipher_error Если текст пустой или содержит недопустимые символы
     */
    std::wstring encrypt(const std::wstring& text);
    
    /**
     * @brief Метод дешифрования текста
     * @param cipher_text Зашифрованный текст
     * @return Расшифрованная строка
     * @throw table_cipher_error Если текст пустой или содержит недопустимые символы
     */
    std::wstring decrypt(const std::wstring& cipher_text);
    
    /**
     * @brief Проверка корректности ключа
     * @param key Ключ для проверки
     * @throw table_cipher_error Если ключ некорректен
     */
    void validateKey(int key);
};
