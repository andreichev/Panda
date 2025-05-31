#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Codable.hpp"
#include "Rain/Coders/JsonDecoder.hpp"

enum class Time { NIGHT, MORNING, AFTERNOON, EVENING };

const char *timeToString(Time time) {
    switch (time) {
        case Time::NIGHT: {
            return "Night";
        }
        case Time::MORNING: {
            return "Morning";
        }
        case Time::AFTERNOON: {
            return "Afternoon";
        }
        case Time::EVENING: {
            return "Evening";
        }
    }
    return nullptr;
}

Time stringToTime(std::string time) {
    if (time == "Night") {
        return Time::NIGHT;
    } else if (time == "Morning") {
        return Time::MORNING;
    } else if (time == "Afternoon") {
        return Time::AFTERNOON;
    } else {
        return Time::EVENING;
    }
}

struct C : public Rain::Codable {
    std::string test = "TEST STRING";
    int x = 10;

    C() = default;

    C(const std::string &test, int x)
        : test(test)
        , x(x) {}

    RAIN_FIELDS_BEGIN(C)
    RAIN_FIELD(test)
    RAIN_FIELD(x)
    RAIN_FIELDS_END
};

struct B : public Rain::Codable {
    std::string message;
    Time time;
    C object;

    static void encode(const char *key, Rain::Encoder *encoder, const B &data) {
        encoder->beginObject(key);
        encoder->encode("message", data.message);
        const char *strTime = timeToString(data.time);
        encoder->encode("time", strTime);
        encoder->encode("object", data.object);
        encoder->endObject();
    }

    static bool decode(const char *key, Rain::Decoder *decoder, B &data) {
        if (!decoder->beginObject(key)) {
            return false;
        }
        decoder->decode("message", data.message);
        std::string strTime;
        // Rain can't decode const char.
        decoder->decode("time", strTime);
        data.time = stringToTime(strTime);
        decoder->decode("object", data.object);
        decoder->endObject();
        return true;
    }

    B(const char *message, Time time, const C &object)
        : message(message)
        , time(time)
        , object(object) {}

    B()
        : message() {}
};

struct A : public Rain::Codable {
    std::string a;
    std::optional<B> b;

    RAIN_FIELDS_BEGIN(A)
    RAIN_FIELD(a)
    RAIN_FIELD(b)
    RAIN_FIELDS_END

    A(const char *a, std::optional<B> b)
        : a(a)
        , b(b) {}

    A()
        : a()
        , b() {}
};

struct Car : public Rain::Codable {
    std::string mark;
    int year;
    A a;

    RAIN_FIELDS_BEGIN(Car)
    RAIN_FIELD(mark)
    RAIN_FIELD(year)
    RAIN_FIELD(a)
    RAIN_FIELDS_END

    Car(const char *mark, int year, A a)
        : mark(mark)
        , year(year)
        , a(a) {}

    Car()
        : mark()
        , year()
        , a() {}
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
    C c("!@#$$%(", 123);
    B b("Test test testtt", Time::MORNING, c);
    A a("Hello world!", b);
    Car car1("Lamborgini", 2020, a);
    Car car2("Lada", 2014, a);
    Person p1(10, "Bogdan", "Churban", {car1, car2});

    std::stringstream stream;

    Rain::Encoder *encoder = new Rain::JsonEncoder(true);
    encoder->encode(stream, p1);
    std::cout << stream.str() << std::endl;

    Person p2;
    Rain::Decoder *decoder = new Rain::JsonDecoder;
    decoder->decode(stream, p2);

    std::cout << (p1.cars[0].a.b.value().time == p2.cars[0].a.b.value().time) << std::endl;
    std::cout << (p1.cars[0].a.b.value().object.test == p2.cars[0].a.b.value().object.test)
              << std::endl;

    delete encoder;
    return 0;
}
