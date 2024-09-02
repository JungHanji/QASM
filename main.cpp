#include <utils/io.hpp>
#include <Processor/Parsing/processor.hpp>

int main(int argc, char* argv[]){
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " <path to code>" << std::endl;
        return 1;
    }

    qasm::Processor proc(argv[1]);
    auto blocks = proc.blockinize(4);

    proc.process(
        std::get<0>(blocks)
    );
}