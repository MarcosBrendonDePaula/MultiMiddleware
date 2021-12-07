//
// Created by Marcos on 04/08/2021.
//

#include "Buffer.h"
/**
 * Inicializa a instancia do Buffer definindo o tamanho maximo com o informado.
 * @param maxSize Tamanho maximo do buffer.
 */
Buffer::Buffer(int maxSize) {
    this->maxSize = maxSize;
    this->data = std::vector<char>(maxSize,0x0);
    this->actualSize = 0;
}

/**
 * Inicializa a intancia Buffer definindo o texto da string em seu conteudo e definindo seu tamanho maximo.
 * @param str String a ser alocada.
 * @param maxSize Tamanho maximo do buffer.
 */
Buffer::Buffer(std::string str, int maxSize) {
    this->maxSize = maxSize;
    this->data = std::vector<char>(maxSize);
    std::copy(str.begin(), str.end(), this->data.begin());
    this->actualSize = str.size();
}

/**
 * Converte o vector<char> em std::string.
 */
std::string Buffer::toString() {
	if(this->data.size()!=0 && this->actualSize > 0){
		return std::string(&this->data[0],this->actualSize);
	}	
	return "";
}

/**
 * retorna o enderço da primeira posição do vector<char> do buffer.
 */
char* Buffer::getDataAccess() {
    return &data[0];
}

/**
 * retorna o enderço da primeira posição do vector<char> do buffer.
 * @return std::vector<char> : vetor que armazena os bytes recebidos.
 */
const std::vector<char> &Buffer::getData() const {
    return data;
}

/**
 * Copia o vector informado para o vector<char> do buffer.
 */
void Buffer::setData(const std::vector<char> &data) {
    this->data = data;
}

/**
 * Retorna a quantidade maxima de bytes aceitas pelo buffer.
 * @return int : maximo de Bytes aceitados.
 */
int Buffer::getMaxSize() const {
    return maxSize;
}

/**
 * Ajusta a quantidade maxima de bytes aceitas pelo buffer.
 */
void Buffer::setMaxSize(int maxSize) {
    this->maxSize = maxSize;
}

/**
 * Retorna a quantidade de Bytes recebidos no buffer.
 * @return int : quantidade de bytes recebidos.
 */
int Buffer::getActualSize() const {
    return actualSize;
}

/**
 * Ajusta a quantidade de Bytes recebidos no buffer.
 * @return int : quantidade de bytes recebidos.
 */
void Buffer::setActualSize(int actualSize) {
    this->actualSize = actualSize;
}

/**
 * Pemite o acesso ao buffer como um array de bytes.
 * @return char* : endereço de acesso.
 */
char * Buffer::operator[](int indice) {
    return &this->data[indice];
}
