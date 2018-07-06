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

            std::string getStep(unsigned int questID, unsigned int stepID);
            std::string getQuestName(unsigned int questID);
            std::string getDescription(unsigned int questID, unsigned int descriptionID);

        private:
            std::vector<std::string> steps;
            std::vector<std::string> descriptions;

    };

}
}