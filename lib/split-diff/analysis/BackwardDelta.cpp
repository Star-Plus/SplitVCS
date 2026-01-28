//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include "components/DeltaCompressor.h"
#include <iostream>
#include <sstream>

int main()
{
    std::istringstream s1("This is a random sentence"), s2("Hello to the other side"), s3("Can you backward delta decode");
    std::ostringstream out1, out2;

    Split::DeltaCompressor compressor;

    compressor.encode(Split::Blob(s1), Split::Blob(s2, Split::EncodeType::BINARY), Split::Blob(out1));
    std::cout << out1.str() << std::endl;

    compressor.encode(Split::Blob(s2), Split::Blob(s3), Split::Blob(out2));
    std::cout << out2.str() << std::endl;

    std::istringstream os1(out1.str()), os2(out2.str());
    std::ostringstream out;
    compressor.decode(Split::Blob(os1), Split::Blob(os2), Split::Blob(out));

    std::cout << out.str() << std::endl;
}