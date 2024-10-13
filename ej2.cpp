#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

sem_t semaforo; // semaforo para controlar el acceso al cajero
float saldo = 100000.00; // saldo inicial

struct ClienteData {
    int id;
    float monto;
};

void *cliente(void *arg) { // funcion que simula el retiro de dinero de un cliente
    ClienteData *data = (ClienteData *)arg;
    int id = data->id;
    
    sem_wait(&semaforo); // seccion critica
    
    cout << "Ingrese el monto a retirar del cliente " << id + 1 << ": ";
    cin >> data->monto;

    if (data->monto <= saldo) { // si el monto a retirar es menor o igual al saldo
        saldo -= data->monto;
        cout << "Retiro exitoso de Q" << data->monto << endl;
    } else {
        cout << "Saldo insuficiente" << endl;
    }
    cout << "Saldo actual: Q" << saldo << endl;
    sem_post(&semaforo); // fin de la seccion critica
    pthread_exit(NULL); // termina el hilo
    return NULL;
}

int main() {
    int n;
    cout << "Ingrese la cantidad de clientes: ";
    cin >> n;
    pthread_t hilos[n];
    ClienteData *clientes = new ClienteData[n];
    sem_init(&semaforo, 0, 1); // inicializa el semaforo
    for (int i = 0; i < n; i++) {
        clientes[i].id = i;
        pthread_create(&hilos[i], NULL, cliente, (void *)&clientes[i]); // crea el hilo
    }
    for (int i = 0; i < n; i++) {
        pthread_join(hilos[i], NULL); // espera a que los hilos terminen
    }
    sem_destroy(&semaforo); // destruye el semaforo
    delete[] clientes;
    return 0;
}
