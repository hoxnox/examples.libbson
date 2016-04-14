#include <iostream>
#include <iterator>
#include <memory>
#include <cstring>
#include <bson.h>

class Data
{
public:
    int64_t number64;
    int32_t number32;
    std::string str;

    template<class OutIter>
    bool to_bson(OutIter out) const;
};

template<class OutIter> bool
Data::to_bson(OutIter out) const
{
    std::unique_ptr<bson_t, std::function<void(bson_t*)> >
        bson{bson_new(), bson_destroy};

    if (!BSON_APPEND_INT64(bson.get(), "num64", number64)) return false;
    if (!BSON_APPEND_INT32(bson.get(), "num32", number32)) return false;
    if (!BSON_APPEND_UTF8 (bson.get(), "str"  , str.c_str())) return false;
    const uint8_t* begin = bson_get_data(bson.get());
    const uint8_t* end = begin + bson->len;
    std::copy(begin, end, out);
    return true;
}

void 
print_bson(const uint8_t* data, size_t datasz)
{
    std::unique_ptr<bson_t, std::function<void(bson_t*)> >
        bson{bson_new_from_data(data, datasz), bson_destroy};
    std::unique_ptr<char, std::function<void(char*)> >
        json{bson_as_json(bson.get(), NULL), bson_free};
    std::cout << json.get() << std::endl;
}

int
main(int argc, char* argv[])
{
    Data data;
    data.number64 = 42;
    data.number32 = 42*2;
    data.str = "Hello, world!";
    std::vector<uint8_t> bson;
    if (!data.to_bson(std::back_inserter(bson)))
        std::cerr << "Error data conversion." << std::endl;
    print_bson(bson.data(), bson.size());
    return 0;
}

