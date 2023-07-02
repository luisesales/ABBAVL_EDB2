#include <bits/stdc++.h>
using namespace std;

bool found = true;

struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int level;
    int numberOfChildrenLeft;
    int numberOfChildrenRight;
    int height;
    int balanco;

    Node(int k)
    {
        key = k;
        level = 1;
        numberOfChildrenLeft = 0;
        numberOfChildrenRight = 0;
        left = NULL;
        right = NULL;
        height = 1;
        balanco = 0;
    }

    struct Node *insert(struct Node *node, int k)
    {
        found = true;
        if (node == NULL)
        {
            node = new Node(k);
            cout << k << " foi inserido com sucesso!!" << endl;
            return node;
        }

        if (k < node->key)
        {
            node->left = insert(node->left, k);

            if (found)
            {
                node->numberOfChildrenLeft++;
            }
        }
        else if (k > node->key)
        {
            node->right = insert(node->right, k);

            if (found)
            {
                node->numberOfChildrenRight++;
            }
        }
        else
        {
            cout << "Impossível inserir " << k << ", pois ele já existe!" << endl;
            found = false;
        }

        if(node != NULL){
            node->height = getHeight(node);
            node->balanco = updateBalanco(node);
        }

        cout << "Balanço = " << node->balanco << " Chave = " << node->key << endl;

        if (node->balanco > 1) // Right-heavy subtree
        {
            if (k > node->right->key) {
                cout << "Rotação a esquerda" << endl;                
                node = rotacao_esquerda(node);
            }
            else // Right-Left case
            {   
                cout << "Rotação dupla a esquerda" << endl;
                node->right = rotacao_direita(node->right);
                node = rotacao_esquerda(node);
            }
        }
        else if (node->balanco < -1) // Left-heavy subtree
        {
            if (k < node->left->key){
                cout << "Rotação a direita" << endl;
                node = rotacao_direita(node);
            }
            else // Left-Right case
            {
                cout << "Rotação dupla a direita" << endl;
                node->left = rotacao_esquerda(node->left);
                node = rotacao_direita(node);
            }
        }
        

        return node;
    }

    int updateBalanco(struct Node *node)
    {   
        return getHeight(node->right) - getHeight(node->left);
    }

    bool search(struct Node *node, int k)
    {
        bool FOUNDkey;
        if (node == NULL)
            return false;

        if (k < node->key)
        {
            FOUNDkey = search(node->left, k);
        }
        else if (k > node->key)
        {
            FOUNDkey = search(node->right, k);
        }
        else
        {
            cout << "Encontrou!!!!!" << endl;
            return true;
        }
        return FOUNDkey;
    }

    struct Node *getNode(struct Node *node, int k)
    {
        if (node == NULL)
            return node;

        if (k < node->key)
        {
            node = getNode(node->left, k);
        }
        else if (k > node->key)
        {
            node = getNode(node->right, k);
        }
        return node;
    }

    struct Node *findPredecessor(struct Node *node)
    {
        struct Node *current = node->left;
        while (current->right != NULL)
        {
            current = current->right;
        }
        return current;
    }

    struct Node *removeKey(struct Node *node, int k){
        bool imprime = true;
        return removeKey(node, k, imprime);
    }

    struct Node *removeKey(struct Node *node, int k, bool &imprime)
    {
        
        if (node == NULL)
        {
            cout << k << " não está na árvore, não pode ser removido" << endl;
            return node;
        }

        if (node != NULL)
        {
            node->numberOfChildrenLeft = countChildren(node->left);
            node->numberOfChildrenRight = countChildren(node->right);
        }

        if (k < node->key)
        {
            node->left = removeKey(node->left, k, imprime);
            if (found)
            {
                node->numberOfChildrenLeft--;
            }
        }
        else if (k > node->key)
        {
            node->right = removeKey(node->right, k, imprime);
            if (found)
            {
                node->numberOfChildrenRight--;
            }
        }
        else
        {
            if(imprime){
                cout << node->key << " foi removido com sucesso!" << endl;
            }

            if (node->numberOfChildrenLeft == 0 && node->numberOfChildrenRight == 0)
            {   
                cout << "Entrei aqui 1" << endl;
                delete node;
                return NULL;
            }
            else if (node->numberOfChildrenLeft > 0 && node->numberOfChildrenRight == 0)
            {   
                cout << "Entrei aqui 2" << endl;
                struct Node *temp = node;
                node = node->left;
                delete temp;
            }
            else if (node->numberOfChildrenLeft == 0 && node->numberOfChildrenRight > 0)
            {   
                cout << "Entrei aqui 3" << endl;
                struct Node *temp = node;
                node = node->right;
                delete temp;
            }
            else
            {
                struct Node *predecessor;
                predecessor = findPredecessor(node);
                node->key = predecessor->key;
                imprime = false;
                node->left = removeKey(node->left, predecessor->key, imprime);
            }
        }

        if (node != NULL)
        {
            node->numberOfChildrenLeft = countChildren(node->left);
            node->numberOfChildrenRight = countChildren(node->right);
            node->height = getHeight(node);
            node->balanco = updateBalanco(node);
        }

        if (node->balanco > 1) // Right-heavy subtree
        {
            if (node->right->balanco >= 0) // Right-Right case
                return rotacao_esquerda(node);
            else // Right-Left case
            {
                node->right = rotacao_direita(node->right);
                return rotacao_esquerda(node);
            }
        }
        else if (node->balanco < -1) // Left-heavy subtree
        {
            if (node->left->balanco <= 0) // Left-Left case
                return rotacao_direita(node);
            else // Left-Right case
            {
                node->left = rotacao_esquerda(node->left);
                return rotacao_direita(node);
            }
        }

        return node;
    }
    int countChildren(struct Node *node)
    {
        if (node == NULL)
        {
            return 0;
        }
        return 1 + countChildren(node->left) + countChildren(node->right);
    }

    int enesimoElemento(struct Node *node, int pos, int &count)
    {
        int achou = 0;
        if (node->left != NULL)
            achou = enesimoElemento(node->left, pos, count);
        if (achou != 0)
            return achou;

        count++;

        if (count == pos){
            return node->key;
        }
            
        if (node->right != NULL)
            achou = enesimoElemento(node->right, pos, count);
        if (achou != 0)
            return achou;
        return 0;
    }

    int enesimoElemento(int pos, struct Node *node)
    {
        int count = 0;
        return enesimoElemento(node, pos, count);
    }

    int posicao(struct Node *node, int value, int &count)
    {
        int achou = 0;

        // só chama recursão se o nó esquerdo não for vazio
        if (node->left != NULL)
            achou = posicao(node->left, value, count);
        if (achou != 0) // se o nó foi encontrado pare recursão
            return achou;

        // se chave for maior que o valor que estamos procurando pare recursão e retorne 0 simbolizando que não foi achado
        if (node->key > value)
        {
            return 0;
        }

        count++;

        // se chave for igual a que estamos procurando pare recursão e retorne o valor
        if (value == node->key)
        {
            return count;
        }

        // só chama recursão se o nó direito não for vazio
        if (node->right != NULL)
            achou = posicao(node->right, value, count);
        if (achou != 0) // se o nó foi encontrado pare recursão
            return achou;

        return 0;
    }

    int posicao(struct Node *node, int value)
    {
        int count = 0;
        return posicao(node, value, count);
    }

    // Ao chamar a função passar 1 como parâmetro para ser o nível da raiz.
    void calculaNivel(struct Node *node, int l)
    {
        node->level = l;
        // cout << "key = " << node->key << " level = " << node->level << endl;

        if (node->left != NULL)
        {
            node->calculaNivel(node->left, l + 1);
        }
        if (node->right != NULL)
        {
            node->calculaNivel(node->right, l + 1);
        }

        return;
    }

    int calcQtdDigitos(int numero)
    {
        if ((numero / 10) == 0)
        {
            return 1;
        }
        else
        {
            // efetua mais uma chamada recursiva
            return 1 + calcQtdDigitos(numero / 10);
        }
    }

    void imprimeFormato1(struct Node *node)
    {
        for (int ii = 1; ii < node->level; ii++)
        {
            cout << "    ";
        }

        cout << node->key<< endl;

        for (int ii = 4; ii < 40 - node->level * 4 - calcQtdDigitos(node->key); ii++)
        {
            cout << "-";
        }

        cout << endl;

        if (node->left != NULL)
        {
            imprimeFormato1(node->left);
        }
        if (node->right != NULL)
        {
            imprimeFormato1(node->right);
        }

        return;
    }

    void imprimeFormato2(struct Node *node)
    {
        cout << "(" << node->key;

        if (node->left != NULL)
        {
            cout << " ";
            imprimeFormato2(node->left);
        }
        if (node->right != NULL)
        {
            cout << " ";
            imprimeFormato2(node->right);
        }

        cout << ")";
    }

    void imprimeArvore(struct Node *node, int s)
    {
        node->calculaNivel(node, 1);
        if (s == 1)
        {
            imprimeFormato1(node);
        }
        else if (s == 2)
        {
            imprimeFormato2(node);
            cout << endl;
        }
        else cout << "Insira 1 ou 2 para imprimir em um formato específico." << endl;
    }

    string pre_ordem(struct Node *node)
    {
        string concatenados = to_string(node->key);

        if (node->left != NULL)
        {
            concatenados = concatenados + " " + pre_ordem(node->left);
        }
        if (node->right != NULL)
        {
            concatenados = concatenados + " " + pre_ordem(node->right);
        }

        return concatenados;
    }

    int getMedian(struct Node *node)
    {
        int count = 0;
        return getMedian(node, node->countChildren(node), count);
    }

    int getMedian(struct Node *node, int totalNodes, int &count)
    {
        if (node == NULL)
        {
            return -1; // valor inválido que indique erro
        }

        // Percorre a subárvore esquerda
        int median;
        median = getMedian(node->left, totalNodes, count);
        if (median != -1)
        {
            return median;
        }

        // Verifica se o nó atual é o da mediana
        count++;
        if (count == (totalNodes + 1) / 2)
        {
            // medianNode = node;
            return node->key;
        }

        // Percorre a subárvore direita
        return getMedian(node->right, totalNodes, count);
    }
    double getAverage(struct Node *node, double &average)
    {
        if (node == NULL)
        {
            return 0;
        }
        average += node->key;
        getAverage(node->left, average);
        getAverage(node->right, average);
        return average;
    }

    double getAverage(int key, struct Node *node)
    {
        double average = 0;
        node = getNode(node, key);
        if (node != NULL)
        {
            average = getAverage(node, average);
            return average / (node->numberOfChildrenLeft + node->numberOfChildrenRight + 1);
        }
        else
            return -1;
    }

    int getHeight(struct Node *node)
    {
        if (node == NULL)
        {
            return 0;
        }
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    bool ehCompleta(struct Node *node)
    {
        int height = getHeight(node);
        return ehCompleta(node, 1, height);
    }

    bool ehCompleta(struct Node *node, int level, int height)
    {

        if (level == height + 1)
        {
            return (node == NULL);
        }
        if (level == height)
        {
            return true;
        }
        if (node == NULL)
        {
            return false;
        }

        return ehCompleta(node->left, level + 1, height) && ehCompleta(node->right, level + 1, height);
    }

    bool ehCheia(struct Node *node)
    {
        int height = getHeight(node);
        return ehCheia(node, 1, height);
    }

    bool ehCheia(struct Node *node, int level, int height)
    {

        if (level == height + 1)
        {
            return (node == NULL);
        }
        if (node == NULL)
        {
            return false;
        }

        return ehCheia(node->left, level + 1, height) && ehCheia(node->right, level + 1, height);
    }

    struct Node *rotacao_direita(Node *node)
    {
        Node *node_u = node->left;
        node->left = node_u->right;
        node_u->right = node;
        return node_u;
    }

    struct Node *rotacao_esquerda(Node *node)
    {
        Node *node_u = node->right;
        node->right = node_u->left;
        node_u->left = node;
        return node_u;
    }
};

bool is_number(const std::string &s);

int main()
{
    struct Node *node = NULL;
    int numbers;
    fstream file1("arquivo1");

    int numb;
    if (file1.is_open())
    {

        while (file1 >> numb)
        {
            node = node->insert(node, numb);
        }
        node->imprimeArvore(node, 2);
    }
    file1.close();

    cout << endl
         << endl;
    fstream file2("arquivo2");
    string s;
    if (file2.is_open())
    {

        while (file2 >> s)
        {
            if (s == "CHEIA")
            {
                if(node->ehCheia(node)){
                    cout << "A árvore é cheia"<< endl;
                }
                else {
                    cout << "A árvore NÃO é cheia" << endl;
                }
            }
            else if (s == "MEDIANA")
            {
                cout << "Mediana: " << node->getMedian(node) << endl;
            }
            else if (s == "COMPLETA")
            {
                if(node->ehCompleta(node)){
                    cout << "A árvore é completa"<< endl;
                }
                else {
                    cout << "A árvore NÃO é completa" << endl;
                }
            }
            else if (s == "ENESIMO")
            {
                int pos;
                file2 >> pos;
                if(node->enesimoElemento(pos, node) == 0){
                    cout << "A árvore não tem a posição " << pos << endl; 
                }
                else {
                    cout <<"Enésimo elemento na posição " << pos << " em ordem simétrica é o " << node->enesimoElemento(pos, node) << endl;
                }
            }
            else if (s == "POSICAO")
            {
                int value;
                file2 >> value;
                if(node->posicao(node, value) == 0){
                    cout << value <<" não está na árvore!" << endl; 
                }
                else {
                    cout << value <<" em ordem simétrica, está na posição " << node->posicao(node, value) << endl;
                }
            }
            else if (s == "MEDIA")
            {
                int value;
                file2 >> value;
                if(node->getAverage(value, node) == -1){
                    cout << value <<" não está na árvore!" << endl; 
                }
                else {
                    cout << "Média da árvore com raiz " << value << " é igual a " << node->getAverage(value, node) << endl;
                }
            }
            else if (s == "IMPRIMA")
            {
                int value;
                file2 >> value;
                node->imprimeArvore(node, value);
                cout << endl;
            }
            else if (s == "REMOVA")
            {
                int value;
                file2 >> value;
                node = node->removeKey(node, value);
                cout << endl;
            }
            else if (s == "PREORDEM")
            {
                cout << node->pre_ordem(node) << endl;
            }
            else if (s == "INSIRA")
            {
                int value;
                file2 >> value;
                node = node->insert(node, value);
                cout << endl;
            }
            else if (s == "BUSCAR")
            {
                int value;
                file2 >> value;
                if(!node->search(node, value)){
                    cout << value <<" não foi encontrado na árvore!" << endl; 
                }
                cout << endl;
            }
        }
    }
    file2.close();

    return 0;
}

bool is_number(const std::string &s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}
