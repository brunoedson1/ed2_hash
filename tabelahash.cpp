#include "tabelahash.hpp"
#include <iostream>
#include <algorithm>

tabelahash::tabelahash(size_t tamanho)
    : tamanho(tamanho), tabela(tamanho, nullptr) {}

tabelahash::~tabelahash()
{
    for (auto reg : tabela)
    {
        delete reg;
    }
}

class TabelaCheiaException : public std::exception { 
    virtual const char *what() const throw() { 
        return "Tabela hash cheia!"; 
    } 
};

void tabelahash::insere(const std::string &nome, unsigned ano, unsigned ocorrencias)
{
    size_t index = hash(nome);
    if (tabela[index] == nullptr)
    {
        tabela[index] = new registro(nome, ano, ocorrencias);
        std::cout << "Registro inserido na posição " << index << std::endl;
    }
    else
    {
        // Tratamento de colisão usando endereçamento aberto (double hashing)
        size_t newIndex = (index + 1) % tamanho;
        while (tabela[newIndex] != nullptr && newIndex != index)
        {
            newIndex = (newIndex + 1) % tamanho;
        }
        
        if (newIndex != index)
        {
            tabela[newIndex] = new registro(nome, ano, ocorrencias);
            std::cout << "Registro inserido na posição " << newIndex << std::endl;
        }
        else
        {
            std::cerr << "Tabela hash cheia ou erro ao inserir " << nome << std::endl;
        }
    }
}

void tabelahash::imprime_tabela() const
{
    for (size_t i = 0; i < tabela.size(); ++i)
    {
        if (tabela[i] != nullptr)
        {
            std::cout << "Posicao " << i << ": "
                      << tabela[i]->get_nome() << ", "
                      << tabela[i]->get_ano() << ", "
                      << tabela[i]->get_ocorrencias() << std::endl;
        }
        else
        {
            std::cout << "Posicao " << i << ": [Vazio]" << std::endl;
        }
    }
}

std::vector<registro *> tabelahash::maiores_ocorrencias(size_t x) const
{
    std::vector<registro *> registrosNaoNulos;
    for (auto reg : tabela)
    {
        if (reg != nullptr)
        {
            registrosNaoNulos.push_back(reg);
        }
    }

    std::sort(registrosNaoNulos.begin(), registrosNaoNulos.end(), [](registro *a, registro *b) {
        return a->get_ocorrencias() > b->get_ocorrencias();
    });

    if (x > registrosNaoNulos.size())
    {
        x = registrosNaoNulos.size();
    }

    return std::vector<registro *>(registrosNaoNulos.begin(), registrosNaoNulos.begin() + x);
}

registro *tabelahash::pega_registro(const std::string &nome) const
{
    size_t index = acha_registro(nome);
    if (index != -1)
    {
        return tabela[index];
    }
    return nullptr;
}

size_t tabelahash::acha_registro(const std::string &nome) const
{
    size_t index = hash(nome);
    if (tabela[index] != nullptr && tabela[index]->get_nome() == nome)
    {
        return index;
    }
    else
    {
        size_t newIndex = (index + 1) % tamanho;
        while (tabela[newIndex] != nullptr && tabela[newIndex]->get_nome() != nome && newIndex != index)
        {
            newIndex = (newIndex + 1) % tamanho;
        }
        if (tabela[newIndex] != nullptr && tabela[newIndex]->get_nome() == nome)
        {
            return newIndex;
        }
    }
    return -1;
}

size_t tabelahash::hash(const std::string &chave) const
{
    const int base = 31;
    const int mod = 1e9 + 7;
    unsigned long long hashValue = 0;
    for (char c : chave)
    {
        hashValue = (hashValue * base + static_cast<unsigned long long>(c)) % mod;
    }
    return hashValue % tamanho;
}
