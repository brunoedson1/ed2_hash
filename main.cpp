#include "tabelahash.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void carregaCsv(tabelahash &tabela, const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    std::string line;
    // Pular o cabeçalho
    if (std::getline(file, line))
    {
        std::cout << "Cabeçalho ignorado: " << line << std::endl;
    }

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string id, nome, ano_str, genero, ocorrencias_str;

        // Ler os dados do CSV considerando que os campos são separados por vírgula
        if (std::getline(ss, id, ',') &&
            std::getline(ss, nome, ',') &&
            std::getline(ss, ano_str, ',') &&
            std::getline(ss, genero, ',') &&
            std::getline(ss, ocorrencias_str, ','))
        {
            unsigned ano = std::stoi(ano_str);
            unsigned ocorrencias = std::stoi(ocorrencias_str);

            //std::cout << "Inserindo: " << nome << ", " << ano << ", " << ocorrencias << std::endl;
            tabela.insere(nome, ano, ocorrencias);
        }
    }

    file.close();
}

int main()
{
    tabelahash tabela(10);

    carregaCsv(tabela, "NationalNames.csv");

    tabela.imprime_tabela();

    auto topRegistros = tabela.maiores_ocorrencias(10);
    std::cout << "Top 10 nomes com maior ocorrencia:" << std::endl;
    for (auto reg : topRegistros)
    {
        std::cout << reg->get_nome() << ": " << reg->get_ocorrencias() << std::endl;
    }

    return 0;
}
