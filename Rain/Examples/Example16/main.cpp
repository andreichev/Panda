#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Codable.hpp"
#include "Rain/Coders/JsonDecoder.hpp"

enum BodyType { HATCHBACK, SEDAN, CROSSOVER, SUV, SPORT };

struct Car : public Rain::Codable {
    std::string mark;
    int year;
    BodyType bodyType;

    RAIN_FIELDS_BEGIN(Car)
    RAIN_FIELD(mark)
    RAIN_FIELD(year)
    RAIN_FIELD(bodyType)
    RAIN_FIELDS_END

    Car(const char *mark, int year, BodyType bodyType)
        : mark(mark)
        , year(year)
        , bodyType(bodyType) {}

    Car()
        : mark()
        , year()
        , bodyType(BodyType::HATCHBACK) {}
};

struct Person : public Rain::Codable {
    std::string name;
    int age;
    std::vector<Car> cars;
    std::vector<int> x;
    std::string surname;

    RAIN_FIELDS_BEGIN(Person)
    RAIN_FIELD(name)
    RAIN_FIELD(age)
    RAIN_FIELD(x)
    RAIN_FIELD(cars)
    RAIN_FIELD(surname)
    RAIN_FIELDS_END

    Person(int age, const std::string &name, const char *surname, std::vector<Car> cars)
        : age(age)
        , name(name)
        , surname(surname)
        , cars(cars)
        , x({1, 2, 3, 4}) {}

    Person()
        : name()
        , age()
        , x()
        , cars()
        , surname() {}
};

int main() {
    Car car1("Lamborgini", 2020, BodyType::SPORT);
    Car car2("Lada", 2014, BodyType::SEDAN);
    Person p1(10, "Bogdan", "Churban", {car1, car2});

    std::stringstream stream;

    Rain::Encoder *encoder = new Rain::JsonEncoder(true);
    encoder->encode(stream, p1);
    std::cout << stream.str() << std::endl;

    Person p2;
    Rain::Decoder *decoder = new Rain::JsonDecoder;
    decoder->decode(stream, p2);

    std::cout << (p1.cars[0].bodyType == p2.cars[0].bodyType) << std::endl;
    std::cout << (p1.cars[0].mark == p2.cars[0].mark) << std::endl;

    delete encoder;
    return 0;
}
