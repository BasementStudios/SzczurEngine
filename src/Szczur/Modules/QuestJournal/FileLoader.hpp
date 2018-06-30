#include <fstream>
#include <vector>
#include <string>

namespace rat
{
    class FileLoader
    {
        public:
            FileLoader();
            ~FileLoader();

            std::string getStep(const unsigned int &i);
            std::string getDescription(const unsigned int &a);

        private:
            std::vector<std::string> steps;
            std::vector<std::string> descriptions;

    };
}