#include <fstream>
#include <vector>
#include <string>

namespace rat
{
namespace journal
{
    class FileLoader
    {
        public:
            FileLoader();
            ~FileLoader();

            std::string getStep(unsigned int i);
            std::string getDescription(unsigned int a);

        private:
            std::vector<std::string> steps;
            std::vector<std::string> descriptions;

    };

}
}