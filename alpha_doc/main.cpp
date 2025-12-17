#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>

/**
 * @file
 * @author Ганьшин В.А.
 * @version 1.0
 * @date 17.12.2025
 * @brief Главный модуль программы шифрования методом Гронсфельда
 * @details Содержит функции для тестирования модуля шифрования
 */

/**
 * @brief Обработка исключений при тестировании шифрования
 * @param Text Текст для обработки
 * @param key Ключ шифрования
 * @param testName Название теста
 */
void exception_handling(const std::wstring& Text, const std::wstring& key, const std::string& testName)
{
    try {
        std::wstring cipherText;
        std::wstring decryptedText;
        
        std::wcout << L" ТЕСТ: " << testName.c_str() << std::endl;
        std::wcout << L" Ключ: '" << key << L"'" << std::endl;
        std::wcout << L" Текст: '" << Text << L"'" << std::endl;
        
        // Создаем объект шифра
        modAlphaCipher cipher(key);
        
        // Шифруем текст
        cipherText = cipher.encrypt(Text);
        
        // Расшифровываем текст
        decryptedText = cipher.decrypt(cipherText);
        
        // Выводим результаты
        std::wcout << L"   РЕЗУЛЬТАТ: УСПЕХ" << std::endl;
        std::wcout << L"   Зашифрованный текст: " << cipherText << std::endl;
        std::wcout << L"   Расшифрованный текст: " << decryptedText << std::endl;
        
        // Проверяем корректность
        if(Text == decryptedText) {
            std::wcout << L"   Проверка: расшифровка корректна" << std::endl;
        } else {
            std::wcout << L"   Проверка: расшифровка некорректна" << std::endl;
        }
        
    } catch (const cipher_error& e) {
        // Обрабатываем исключения шифрования
        std::wcout << L"    ОШИБКА ШИФРОВАНИЯ: " << e.what() << std::endl;
        
        // Детализируем тип ошибки
        std::string error_msg = e.what();
        if (error_msg.find("Пустой ключ") != std::string::npos) {
            std::wcout << L"       ТИП ОШИБКИ: Пустой ключ" << std::endl;
        } else if (error_msg.find("Недопустимый символ в ключе") != std::string::npos) {
            std::wcout << L"       ТИП ОШИБКИ: Недопустимые символы в ключе" << std::endl;
        } else if (error_msg.find("Пустой текст") != std::string::npos) {
            std::wcout << L"       ТИП ОШИБКИ: Пустой текст" << std::endl;
        } else if (error_msg.find("недопустимые символы") != std::string::npos) {
            std::wcout << L"       ТИП ОШИБКИ: Недопустимые символы в тексте" << std::endl;
        } else if (error_msg.find("не содержит символов русского алфавита") != std::string::npos) {
            std::wcout << L"       ТИП ОШИБКИ: Отсутствуют русские буквы" << std::endl;
        } else if (error_msg.find("индекс символа") != std::string::npos) {
            std::wcout << L"       ТИП ОШИБКИ: Ошибка индексации символов" << std::endl;
        } else {
            std::wcout << L"       ТИП ОШИБКИ: Неизвестная ошибка шифрования" << std::endl;
        }
        
    } 
    std::wcout << std::endl;
}

/**
 * @brief Тестирование корректных случаев шифрования
 */
void testCorrectCases()
{
    std::wcout << L"ТЕСТИРОВАНИЕ КОРРЕКТНЫХ ДАННЫХ" << std::endl;
    
    exception_handling(L"ТИМПЛБДВА", L"ДОЖДИ", "Корректные данные 1");
    exception_handling(L"ШИФРГРОНСФЕЛЬДА", L"СЕВЕР", "Корректные данные 2");
    exception_handling(L"ИСКЛЮЧЕНИЯ", L"КИТ", "Корректные данные 3");
    exception_handling(L"ПРОГРАММИРОВАНИЕ", L"АЛГОРИТМ", "Корректные данные 4");
    exception_handling(L"ПРИВЕТМИР", L"КОД", "Текст с пробелом тест 5");
}

/**
 * @brief Тестирование ошибочных случаев шифрования
 */
void testErrorCases()
{
    std::wcout << L"ТЕСТИРОВАНИЕ ОШИБОЧНЫХ СЛУЧАЕВ" << std::endl;
    
    // Тест с пустым ключом
    exception_handling(L"ТЕКСТ", L"", "Пустой ключ");
    
    // Тест с недопустимыми символами в ключе
    exception_handling(L"ТЕКСТ", L"КЛЮЧ458", "Ключ с цифрами");
    
    // Тест с пустым текстом
    exception_handling(L"", L"КЛЮЧ", "Пустой текст");
    
    // Тест с недопустимыми символами в тексте
    exception_handling(L"ТЕКСТ895", L"КЛЮЧ", "Текст с цифрами");
    
    // Тест с английскими буквами (не входят в русский алфавит)
    exception_handling(L"TEXT", L"KEY", "Английские буквы");
    
    // Тест со специальными символами
    exception_handling(L"ТЕКСТ!", L"КЛЮЧ", "Текст с восклицательным знаком");
    
    // Тест с ключом из спецсимволов
    exception_handling(L"ТЕКСТ", L"КЛЮЧ!?!", "Ключ со спецсимволами");
}

/**
 * @brief Демонстрация возможных типов ошибок
 */
void demonstrateErrorTypes()
{
    std::wcout << L"ДЕМОНСТРАЦИЯ ТИПОВ ОШИБОК" << std::endl;
    
    std::wcout << L"Возможные типы ошибок:" << std::endl;
    std::wcout << L"1.  Пустой ключ" << std::endl;
    std::wcout << L"2.  Недопустимые символы в ключе" << std::endl; 
    std::wcout << L"3.  Пустой текст" << std::endl;
    std::wcout << L"4.  Недопустимые символы в тексте" << std::endl;
    std::wcout << L"5.  Отсутствуют русские буквы" << std::endl;
    std::wcout << L"6.  Ошибка индексации символов" << std::endl;
    std::wcout << std::endl;
}

/**
 * @brief Главная функция программы
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения программы
 */
int main(int argc, char** argv)
{
    // Установка локали для поддержки русского языка
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    std::wcerr.imbue(std::locale("ru_RU.UTF-8"));
    
    std::wcout << L" ПРОГРАММА ШИФРОВАНИЯ МЕТОДОМ ГРОНСФЕЛЬДА" << std::endl;
    std::wcout << std::endl;
    
    // Демонстрация типов ошибок
    demonstrateErrorTypes();
    
    // Тестируем ошибочные случаи
    testErrorCases();
    
    // Тестируем корректные случаи
    testCorrectCases();
    
    std::wcout << L"Все тесты завершены." << std::endl;
    
    return 0;
}
