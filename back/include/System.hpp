#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
class System
{
  public:
    static std::string exec(const char *command)
    {
        // Ouvre un pipe pour lire la sortie de la commande
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);

        if (!pipe)
        {
            throw std::runtime_error("popen() a échoué !");
        }

        // Lit la sortie de la commande
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }

        return result;
    }
    static bool writeFile(const std::string &contenu, const std::string &cheminFichier)
    {
        std::ofstream fichier(cheminFichier);

        if (!fichier)
        {
            std::cerr << "Erreur : impossible d'ouvrir le fichier \"" << cheminFichier << "\" pour l'écriture."
                      << std::endl;
            return false;
        }

        fichier << contenu;

        if (!fichier)
        {
            std::cerr << "Erreur : échec lors de l'écriture dans le fichier." << std::endl;
            return false;
        }

        fichier.close();
        return true;
    }
};

#endif // !DEBUG
