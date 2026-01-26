//
// Created by lightum on 1/20/26.
//

#include "ByteEncoder.h"
#include <google/vcencoder.h>
#include <sstream>
#include <fstream>

#include "ByteDecoder.h"

namespace Split {

    void ByteEncoder::encode(const std::istream &v1, const std::istream &v2, std::ostream &out) {
        const auto v1Data = v1.rdbuf();
        const auto v2Data = v2.rdbuf();

        std::ostringstream v1SS, v2SS;
        v1SS << v1Data;
        v2SS << v2Data;

        open_vcdiff::VCDiffEncoder encoder(
            (v1SS.str().c_str()), v1SS.str().size()
        );

        encoder.SetFormatFlags(open_vcdiff::VCD_FORMAT_INTERLEAVED);

        std::string outDelta;

        if (!encoder.Encode(v2SS.str().c_str(), v2SS.str().size(), &outDelta)) {
            throw std::runtime_error("Failed to encode delta");
        }

        out << outDelta;
    }

    std::string ByteEncoder::encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2,
        std::string& out)
    {
        auto decoded = ByteDecoder::decode(base, deltas);

        std::fstream v2File(v2, std::ios::in | std::ios::binary);
        std::stringstream v2SS;
        v2SS << v2File.rdbuf();


        open_vcdiff::VCDiffEncoder encoder(
            decoded.str().c_str(), decoded.str().size()
        );

        encoder.SetFormatFlags(open_vcdiff::VCD_FORMAT_INTERLEAVED);
        std::string outDelta;

        if (!encoder.Encode(v2SS.str().c_str(), v2SS.str().size(), &outDelta))
        {
            throw std::runtime_error("Failed to encode delta");
        }

        std::fstream outFile(out, std::ios::out | std::ios::binary);
        outFile << outDelta;
        outFile.close();

        return out;
    }
}
