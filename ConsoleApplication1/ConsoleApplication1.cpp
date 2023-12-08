#include <iostream>
#include <fstream>
#include <zip.h>
#include <string>
#include <vector>

// Функция для извлечения файлов из архива
void extractAllFiles(const char* archiveFilename, const char* extractionPath) {
    struct zip* archive = zip_open(archiveFilename, 0, nullptr);
    if (archive == nullptr) {
        std::cerr << "Unable to open archive: " << archiveFilename << std::endl;
        return;
    }

    int numEntries = zip_get_num_entries(archive, 0);

    for (int i = 0; i < numEntries; ++i) {
        struct zip_stat stat;
        zip_stat_init(&stat);

        zip_stat_index(archive, i, 0, &stat);

        struct zip_file* file = zip_fopen_index(archive, i, 0);
        if (file == nullptr) {
            std::cerr << "Unable to open file in archive at index " << i << std::endl;
            continue;
        }

        char* buffer = new char[stat.size];
        zip_fread(file, buffer, stat.size);
        zip_fclose(file);

        std::string outputPath = extractionPath + std::string(stat.name);

        // Создаем директории, если они не существуют
        size_t lastSlash = outputPath.find_last_of("/");
        if (lastSlash != std::string::npos) {
            std::string directory = outputPath.substr(0, lastSlash);
            system(("mkdir \"" + directory + "\"").c_str());
        }

        std::ofstream outputFile(outputPath, std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Unable to create file: " << outputPath << std::endl;
            delete[] buffer;
            continue;
        }

        outputFile.write(buffer, stat.size);
        outputFile.close();
        delete[] buffer;
    }

    zip_close(archive);
    std::cout << "All files extracted successfully to: " << extractionPath << std::endl;
}


// Функция для добавления файлов в архив
bool addToArchive(const char* archiveFilename, const std::vector<std::string>& filesToAdd) {
    struct zip* archive = zip_open(archiveFilename, ZIP_CREATE, nullptr);
    if (archive == nullptr) {
        std::cerr << "Unable to create archive: " << archiveFilename << std::endl;
        return false;
    }

    for (const auto& fileToAdd : filesToAdd) {
        struct zip_source* source = zip_source_file(archive, fileToAdd.c_str(), 0, 0);
        if (source == nullptr) {
            std::cerr << "Unable to open file: " << fileToAdd << std::endl;
            zip_close(archive);
            return false;
        }

        const std::string entryName = fileToAdd.substr(fileToAdd.find_last_of("/\\") + 1);

        if (zip_file_add(archive, entryName.c_str(), source, ZIP_FL_OVERWRITE) < 0) {
            std::cerr << "Error adding file to archive: " << fileToAdd << std::endl;
            zip_source_free(source);
            zip_close(archive);
            return false;
        }

        std::cout << "File added to archive: " << fileToAdd << std::endl;
    }

    zip_close(archive);
    std::cout << "All files added to archive successfully." << std::endl;

    return true;
}

int main() {
    const char* archiveFilename = "C:\\test\\archive.zip"; //Измени на путь к архиву в который нужно добавить файлы или же извлечь


    //std::vector<std::string> filesToAdd = {
    //    "C:/test/test1.gif",
    //    "C:/test/test2.jpg",
    //    "C:/test/test3.jpg",
    //};  //Укажи пути к файлам которые хочешь добавить в архив


    //Вызов функции для добавления файлов в архив
    //if (addToArchive(archiveFilename, filesToAdd)) {
    //    std::cout << "Archive created and files added successfully." << std::endl;
    //}
    //else {
    //    std::cerr << "Failed to create archive or add files." << std::endl;
    //}


    //const char* extractionPath = "C:\\test\\filesFrom\\"; //Укажи путь к папке в которую хочешь извлечь файлы из архива
    //extractAllFiles(archiveFilename, extractionPath); \\Вызов функции для извлечения файлов

    return 0;
}
