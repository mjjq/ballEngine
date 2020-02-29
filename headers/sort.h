#ifndef SORT_H
#define SORT_H

class Sort
{
public:
    template <typename T>
    static void insertionSort(std::vector<T> & data)
    {
        for(int i=1; i<data.size(); ++i)
            for(int j=i; j > 0 && data[j] < data[j-1]; --j)
                std::swap(a[j], a[j-1]);
    }
};

#endif // SORT_H
