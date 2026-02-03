//
// Created by lightum on 1/20/26.
//

#include "ByteEncoder.h"
#include <google/vcencoder.h>
#include <sstream>
#include <fstream>
#include <vector>

#include "ByteDecoder.h"

namespace Split {

    std::string ByteEncoder::encode(const std::string& base, const std::string& out)
    {
        std::fstream outFile(out, std::ios::out | std::ios::binary);
        std::fstream baseFile(base, std::ios::binary | std::ios::in);

        outFile << baseFile.rdbuf();

        outFile.close();
        baseFile.close();

        return out;
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
