//
// Created by lightum on 1/20/26.
//

#include "ByteDecoder.h"

#include <fstream>
#include <istream>
#include <sstream>
#include <google/vcdecoder.h>

namespace Split {

    void ByteDecoder::decode(const std::istream &base, const std::istream &delta, std::ostream &out) {

        const auto baseData = base.rdbuf();
        const auto deltaData = delta.rdbuf();

        std::ostringstream baseSS, deltaSS;
        baseSS << baseData;
        deltaSS << deltaData;

        open_vcdiff::VCDiffDecoder decoder;

        std::string output;
        if (!decoder.Decode(baseSS.str().c_str(),
            baseSS.str().size(),
            deltaSS.str(),
            &output)
        ) {
            throw std::runtime_error("Error while decoding");
        }

        out << output;
    }

    void ByteDecoder::decode(const std::string& base, std::stack<std::string>& deltas, std::string& out)
    {
        std::fstream outFile(out, std::ios::out);
        if (!outFile)
        {
            throw std::runtime_error("Failed to open output file");
        }

        auto stream = decode(base, deltas);
        if (!stream)
        {
            outFile.close();
            throw std::runtime_error("Failed to open output file");
        }

        outFile << stream.rdbuf();
        outFile.close();
    }

    std::stringstream ByteDecoder::decode(const std::string& base, std::stack<std::string>& deltas)
    {

        std::fstream baseFile(base, std::ios::in);

        std::stringstream middle;
        middle << baseFile.rdbuf();

        while (!deltas.empty())
        {
            std::fstream deltaFile(deltas.top(), std::ios::in | std::ios::binary);
            deltas.pop();

            std::ostringstream deltaStream;
            deltaStream << deltaFile.rdbuf();

            open_vcdiff::VCDiffDecoder decoder;

            std::string output;

            std::string dictionary = middle.str();

            if (!decoder.Decode(
                dictionary.data(),
                dictionary.size(),
                deltaStream.str(),
                &output)
            ) {
                throw std::runtime_error("Error while decoding");
            }

            // Replace contents of middle to contents of output
            middle.clear();
            middle << output;
        }

        return middle;

    }
}
