#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

class IStatistics {
public:
    virtual ~IStatistics() {}

    virtual void update(double next) = 0;
    virtual double eval() const = 0;
    virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
    Min() : m_min{std::numeric_limits<double>::max()} {
    }

    void update(double next) override {
        if (next < m_min) {
            m_min = next;
        }
    }

    double eval() const override {
        return m_min;
    }

    const char * name() const override {
        return "min";
    }

private:
    double m_min;
};

class Max : public IStatistics {
public:
    Max() : m_max(std::numeric_limits<double>::lowest()) {
    }

    void update(double next) override {
        if (next > m_max) {
            m_max = next;
        }
    }

    double eval() const override {
        return m_max;
    }

    const char * name() const override {
        return "max";
    }

private:
    double m_max;
};

class Mean : public IStatistics {
public:
    Mean() : m_sum(0), m_count(0) {
    }

    void update(double next) override {
        m_sum += next;
        ++m_count;
    }

    double eval() const override {
        return m_count ? m_sum / m_count : 0;
    }

    const char * name() const override {
        return "mean";
    }

private:
    double m_sum;
    size_t m_count;
};

class Pct90 : public IStatistics {
public:
    Pct90() = default;

    void update(double next) override {
        sortedArr.push_back(next);
    }

    double eval() const override {
        std::vector<double> sortedArrCopy = sortedArr; // Создаем копию массива для сортировки
        std::sort(sortedArrCopy.begin(), sortedArrCopy.end());
        double index = 0.9 * (sortedArrCopy.size() - 1);
        if (index != static_cast<int>(index)) {
            int lowerIndex = static_cast<int>(index);
            int upperIndex = lowerIndex + 1;
            double lowerValue = sortedArrCopy[lowerIndex];
            double upperValue = sortedArrCopy[upperIndex];
            return lowerValue + (index - lowerIndex) * (upperValue - lowerValue);
        } else {
            return sortedArrCopy[static_cast<int>(index)];
        }
    }

    const char * name() const override {
        return "Pct90";
    }

private:
    std::vector<double> sortedArr;
};

class Pct95 : public IStatistics {
public:
    Pct95() = default;

    void update(double next) override {
        sortedArr.push_back(next);
    }

    double eval() const override {
        std::vector<double> sortedArrCopy = sortedArr; // Создаем копию массива для сортировки
        std::sort(sortedArrCopy.begin(), sortedArrCopy.end());
        double index = 0.95 * (sortedArrCopy.size() - 1);
        if (index != static_cast<int>(index)) {
            int lowerIndex = static_cast<int>(index);
            int upperIndex = lowerIndex + 1;
            double lowerValue = sortedArrCopy[lowerIndex];
            double upperValue = sortedArrCopy[upperIndex];
            return lowerValue + (index - lowerIndex) * (upperValue - lowerValue);
        } else {
            return sortedArrCopy[static_cast<int>(index)];
        }
    }

    const char * name() const override {
        return "Pct95";
    }

private:
    std::vector<double> sortedArr;
};

int main() {
    const size_t statistics_count = 5;
    IStatistics *statistics[statistics_count];

    statistics[0] = new Min{};
    statistics[1] = new Max();
    statistics[2] = new Mean();
    statistics[3] = new Pct90();
	statistics[4] = new Pct95();

    double val = 0;
    while (std::cin >> val) {
        for (size_t i = 0; i < statistics_count; ++i) {
            statistics[i]->update(val);
        }
    }

    if (!std::cin.eof() && !std::cin.good()) {
        std::cerr << "Invalid input data\n";
        return 1;
    }

    for (size_t i = 0; i < statistics_count; ++i) {
        std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
    }

    for (size_t i = 0; i < statistics_count; ++i) {
        delete statistics[i];
    }

    return 0;
}
