#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <fstream> // для работы с файлами

using namespace std;
using namespace std::chrono;
const int RANGE = 20000; // максимальное значение элемента для случайного заполнения
void generateRandomArray(vector<int>& arr, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr.push_back(rand() % RANGE);
    }
}

void generateSortedArray(vector<int>& arr, int n) {
    for (int i = 0; i < n; i++) {
        arr.push_back(i);
    }
}

void generateAlmostSortedArray(vector<int>& arr, int n) {
    int sorted = n * 0.9;
    int unsorted = n - sorted;
    for (int i = 0; i < sorted - 1; i++) {
        arr.push_back(i);
    }
    for (int i = unsorted; i >= 0; i--) {
        arr.push_back(i);
    }
}

void generateReverseSortedArray(vector<int>& arr, int n) {
    for (int i = n; i > 0; i--) {
        arr.push_back(i);
    }
}


// Реализация алгоритмов сортировки
// Функция сортировки выбором
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

// Функция сортировки вставками
void insertionSort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int el = arr[i]; // Сохраняем текущий элемент
        // Сдвигаем элементы, которые больше текущего
        for (int j = i; j > 0 && arr[j - 1] > el; j--) {
            arr[j] = arr[j - 1];
            arr[j - 1] = el; // Вставляем текущий элемент на правильную позицию

        }
    }
}


void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;//граница 1
    int n2 = r - m;//граница 2
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[m + 1 + j];
    }
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void shellSort_Shell(vector<int>& arr) {
    for (int s = arr.size() / 2; s > 0; s /= 2) { // Цикл по шагам s
        for (int i = s; i < arr.size(); ++i) {
            // Сравниваем текущий элемент с элементами, находящимися на расстоянии s
            for (int j = i - s; j >= 0 && arr[j] > arr[j + s]; j -= s) {
                // Обмен значений
                int temp = arr[j];
                arr[j] = arr[j + s];
                arr[j + s] = temp;
            }
        }
    }
}

void shellSort_Hibbard(vector<int>& arr) {
    int i, j, k, increment, temp;
    int val;
    // Вычисление начального шага
    val = (int)log((float)arr.size() + 1) / log((float)2);
    increment = pow((float)2, val) - 1;
    while (increment > 0)
    {
        for (i = 0; i < increment; i++)
        {
            for (j = 0; j < arr.size(); j += increment)
            {
                temp = arr[j];
                for (k = j - increment; k >= 0 && temp < arr[k]; k -= increment)
                {
                    arr[k + increment] = arr[k]; // Сдвигаем элементы вправо
                }
                arr[k + increment] = temp; // Вставляем элемент temp в правильное место
            }
        }
        val--; // Уменьшаем val для следующего шага

        // Вычисление нового значения шага
        if (increment != 1)
            increment = pow((float)2, val) - 1;
        else
            increment = 0;
    }
}

// Сортировка Шелла с последовательностью Пратта
void shellSort_Pratt(vector<int>& arr) {
    vector<int> gaps;
    int size = arr.size();

    // Генерация последовательности Пратта
    for (int i = 1; i < size; i *= 2)
    {
        for (int j = i; j < size; j *= 3)
        {
            gaps.push_back(j);
        }
    }

    // Сортировка шагов по убыванию
    insertionSort(gaps);

    // Сортировка массива по каждому шагу
    for (int gap : gaps)
    {
        for (int i = gap; i < size; i++)
        {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }

}

// Функция для разбиения массива на две части
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Определяем опорный элемент как последний элемент массива
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++; // Увеличиваем индекс меньшего элемента
            // Обмен значений
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    // Меняем местами опорный элемент с элементом на позиции i + 1
    int tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return (i + 1); // Возвращаем индекс опорного элемента
}

// Функция быстрой сортировки
void quickSort(vector<int>& arr, int low, int high) {

    if (low < high) {

        int pi = partition(arr, low, high); // Вычисление индекса опорного элемента
        quickSort(arr, low, pi - 1); // Быстрая сортировка для элементов до опорного
        quickSort(arr, pi + 1, high); // Быстрая сортировка для элементов после опорного
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;       // Инициализируем наибольший элемент как корень
    int left = 2 * i + 1;  // Левый дочерний элемент
    int right = 2 * i + 2; // Правый дочерний элемент

    // Если левый дочерний элемент больше корня
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // Если правый дочерний элемент больше, чем самый большой элемент на данный момент
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Если самый большой элемент не корень
    if (largest != i) {
        int tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;


        // Рекурсивно преобразуем затронутое поддерево
        heapify(arr, n, largest);
    }
}

// Функция пирамидальной сортировки
void heapSort(vector<int>& arr) {
    int n = arr.size();
    // Построение max-heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Извлечение элементов из кучи по одному
    for (int i = n - 1; i >= 0; i--) {
        // Перемещаем текущий корень в конец
        int tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;

        // Вызываем heapify на уменьшенной куче
        heapify(arr, i, 0);
    }
}

double measureSortTime(vector<int>& arr, void (*sortFunc)(vector<int>&)) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

// Функция записи в файл
void recordToFile(ofstream& data_file, const string& sortName, const string& arrayType, int size, double timeTaken) {
    data_file << sortName << ", " << arrayType << ", " << size << ", " << timeTaken << " сек\n";
}

// Основная функция для запуска сортировок и записи результатов
void runAndRecordSorts() {
    vector<int> sizes = { 1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000 };
    vector<string> arrayTypes = { "Случайный", "Почти отсортированный", "Отсортированный в обратном порядке", "Отсортированный" };
    vector<void (*)(vector<int>&)> sortFunctions = {
        selectionSort, insertionSort, bubbleSort,
        [](vector<int>& arr) { mergeSort(arr, 0, arr.size() - 1); },
        shellSort_Shell,
        shellSort_Hibbard,
        [](vector<int>& arr) { quickSort(arr, 0, arr.size() - 1); },
        heapSort
    };
    vector<string> sortNames = {
        "Сортировка выбором", "Сортировка вставками", "Сортировка пузырьком",
        "Сортировка слиянием", "Сортировка Шелла (последовательность Шелла)",
        "Сортировка Шелла (последовательность Хиббарда)",
        "Быстрая сортировка", "Пирамидальная сортировка"
    };

    // Создание и открытие текстового файла
    ofstream data_file("DATA.txt");
    if (!data_file.is_open()) {
        cerr << "Не удалось открыть файл для записи!" << endl;
        return;
    }

    cout << "Начинаю запись в файл DATA.txt...\n";
    data_file << "Тип сортировки, Тип массива, Количество элементов, Время выполнения (сек)\n";

    for (int sortIdx = 0; sortIdx < sortFunctions.size(); ++sortIdx) {
        for (int arrayIdx = 0; arrayIdx < arrayTypes.size(); ++arrayIdx) {
            for (int size : sizes) {
                vector<int> arr;
                switch (arrayIdx) {
                case 0: generateRandomArray(arr, size); break;
                case 1: generateAlmostSortedArray(arr, size); break;
                case 2: generateReverseSortedArray(arr, size); break;
                case 3: generateSortedArray(arr, size); break;
                }

                double timeTaken = measureSortTime(arr, sortFunctions[sortIdx]);
                recordToFile(data_file, sortNames[sortIdx], arrayTypes[arrayIdx], size, timeTaken);

                // Вывод информации о каждом записанном элементе в консоль
                cout << sortNames[sortIdx] << ", " << arrayTypes[arrayIdx] << ", " << size << ", " << timeTaken << " сек\n";
            }
        }
    }

    data_file.close(); // Закрытие файла
    cout << "Запись завершена. Данные сохранены в DATA.txt\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));

    runAndRecordSorts();

    cout << "Результаты сортировок записаны в файл DATA.csv" << endl;
    system("pause");
    return 0;
}
