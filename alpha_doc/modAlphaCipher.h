#pragma once
#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>

/**
 * @file
 * @author Ганьшин В.А.
 * @version 1.0
 * @date 17.12.2025
 * @brief Заголовочный файл для модуля шифрования методом Гронсфельда
 * @warning Это документирование программы лабораторной работы
 */

/**
 * @brief Класс для исключений шифрования
 * @details Наследуется от std::exception для интеграции со стандартной системой исключений
 */
class cipher_error : public std::exception {
private:
    std::string message;
public:
    /**
     * @brief Конструктор исключения
     * @param msg Сообщение об ошибке
     */
    explicit cipher_error(const std::string& msg) : message(msg) {}
    
    /**
     * @brief Получить сообщение об ошибке
     * @return Указатель на строку с сообщением об ошибке
     */
    const char* what() const noexcept override {
        return message.c_str();
    }
};

/**
 * @brief Шифрование методом Гронсфельда для русского языка
 * @details Ключ устанавливается в конструкторе.
 * Для зашифровывания и расшифровывания предназначены методы encrypt и decrypt.
 * @warning Реализация только для русского языка
 */
class modAlphaCipher
{
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; ///< Алфавит русского языка
    std::map <wchar_t,int> alphaNum; ///< Ассоциативный массив "символ-номер"
    std::vector <int> key; ///< Ключ шифрования в числовом виде
    
    /**
     * @brief Преобразование строки в числовой вектор
     * @param s Исходная строка
     * @return Вектор числовых представлений символов
     */
    std::vector<int> convert(const std::wstring& s);
    
    /**
     * @brief Преобразование числового вектора в строку
     * @param v Вектор числовых представлений
     * @return Результирующая строка
     * @throw cipher_error Если индекс выходит за границы алфавита
     */
    std::wstring convert(const std::vector<int>& v);
    
    /**
     * @brief Приведение строки к верхнему регистру
     * @param s Исходная строка
     * @return Строка в верхнем регистре
     */
    std::wstring toUpper(const std::wstring& s);
    
public:
    /**
     * @brief Запрещенный конструктор без параметров
     */
    modAlphaCipher()=delete;
    
    /**
     * @brief Конструктор для установки ключа
     * @param skey Ключ шифрования
     * @throw cipher_error Если ключ пустой или содержит недопустимые символы
     */
    modAlphaCipher(const std::wstring& skey);
    
    /**
     * @brief Зашифровывание текста
     * @param open_text Открытый текст. Не должен быть пустой строкой.
     * Строчные символы автоматически преобразуются к прописным.
     * Все не-буквы удаляются
     * @return Зашифрованная строка
     * @throw cipher_error Если текст пустой или содержит недопустимые символы
     */
    std::wstring encrypt(const std::wstring& open_text);
    
    /**
     * @brief Расшифровывание текста
     * @param cipher_text Зашифрованный текст. Не должен быть пустой строкой.
     * Должен содержать только буквы русского алфавита
     * @return Расшифрованная строка
     * @throw cipher_error Если текст пустой или содержит недопустимые символы
     */
    std::wstring decrypt(const std::wstring& cipher_text);
};
