#include <iostream>
#include <conio.h>
#include <fstream>
using namespace std;

void pickarch(string& datosn,string& datos, string& archivo){                           //funcion encargada de retornal (por referencia) los nombres de los archivos donde se sacan y guardan datos.
    ofstream escribir;
    ifstream leer;
   
    cout<<"Escribir nombre del archivo donde se encuentran los datos de los nombres sin extensiones: ";
    cin>>datosn;            //cin del nombre del archivo donde se encuentran los nombres
    datosn+=".txt";
    leer.open(datosn);
    while(!leer){           //control de error del datosn
        leer.close();
        cout<<datosn<<" no es un archivo de datos valido, por favor escriba correctamente el nombre del archivo: ";
        cin>>datosn;
        datosn+=".txt";
        leer.open(datosn);
    }
    leer.close();

    cout<<"Escribir nombre del archivo donde se encuentran los datos de los lotes sin extensiones: ";
    cin>>datos;             //cin del archivo donde se encuentran los datos
    datos+=".txt";
    leer.open(datos);
    while(!leer){           //control de error del datos
        leer.close();
        cout<<datos<<" no es un archivo de datos valido, por favor escriba correctamente el nombre del archivo: ";
        cin>>datos;
        datos+=".txt";
        leer.open(datos);
    }
    leer.close();
    cout<<"Elegir nombre del archivo donde se guardara el resumen: ";
    cin>>archivo;           //cin del archivo donde se guardara a lo largo del programa
    archivo+=".txt";
    escribir.open(archivo);
    while(!escribir){       //control de error del archivo de guardado
        escribir.close();
        cout<<archivo<<" no es un archivo de datos valido, por favor escriba correctamente el nombre del archivo: ";
        cin>>archivo;
        archivo+=".txt";
        escribir.open(archivo);
    }
    escribir.close();
    
}

void carg_dat(float kg[8][5],float km[8][5],string datos){                              //mete los kg y km recorridos en las matrices y celdas correspondientes
    int cliente,producto;
    float kilog,kilom;
    ifstream leer;
    leer.open(datos);
    while(leer>>cliente){        //ciclo mientras pueda leer clientes                                                                 
    leer>>producto>>kilog>>kilom;//lee el producto, la cantidad de kg y km
    kg[cliente][producto]+=kilog;//asigna los kg a la celda correspondiente
    km[cliente][producto]+=kilom;//asigna los km a la celda correspondiente
    }

    leer.close();
}

void carg_nom(string cliente[],string producto[],string datosn){                        //carga los nombres en los vectores cliente y producto
    ifstream leer;
    leer.open(datosn);
    for(int i=0;i<8;i++){       //ciclo entre los 8 clientes
        leer>>cliente[i];       //asigna el nombre del cliente a la celda correspondiente
    }
    for (int i=0;i<5;i++){      //ciclo entre los 5 productos
        leer>>producto[i];      //asigna el nombre del producto a la celda correspondiente
    }
    leer.close();
}

void kg13k(float kg[8][5],string cliente[],string producto[],string archivo){           //escribe el cliente y los productos que pasaron 13000kg
    ofstream save;
    save.open(archivo,ios::app);
    for (int cl=0;cl<8;cl++){               //ciclo entre los 8 clientes
        save<<"\t"<<cliente[cl]<<":";       //imprime el nombre del cliente
        for (int prod=0;prod<5;prod++){     //ciclo entre los 5 productos
            if(kg[cl][prod]>13000){         //si hay mas de 13000kg imprime el nombre del producto.
                save<<" "<<producto[prod];
            }
        }
        save<<".\n";
    }
    save.close();
}

int maxcliente(float kg[8][5]){                             //devuelve el cliente con más productos >13000kg
    int cliente[8]{};

    for(int i=0;i<8;i++){           //si el cliente tiene un producto con mas de 13000kg
        for(int o=0;o<5;o++){
            if(kg[i][o]>13000){
                cliente[i]++;       //agrega uno al contador
            }
        }
    }
    int cmax=cliente[0];            
    for (int i=0;i<8;i++){           //busca y retorna el cliente con mas productos mayores de 13000kg
        if(cliente[cmax]<cliente[i]){
            cmax=i;
        }
    }
    return cmax;
}

void ordenar_vector(float v[],int dim){                     //ordena al vector
    float mem=0;
    for(int i=dim;i>0;i--){
        int ind=dim-1;           //toma la ultima celda, luego la anteultimo, luego la antepenultimo, etc
        for(;ind>0;ind--){       
            if(v[ind]<v[ind-1]){    //si el valor es menor al de arriba
                mem=v[ind];
                v[ind]=v[ind-1];
                v[ind-1]=mem;       //las cambia de lugar.
            }
        }
    }
}

string traducir_vec(float mat[][5],
                    float v[],
                    int numv,
                    int numm, 
                    string prod[],
                    int& idprod){                           //compara un vector ordenado y la fila de la matriz de donde salio. retorna el nombre del producto asociado 
    for (int i=0;i<5;i++){
        if(v[numv]==mat[numm][i]){
            idprod = i;
            return prod[i];

        }
    }
    return "error";
}

void list_ascen(float km[8][5],                             //imprime en el txt los km por producto de un cliente a eleccion 
                int cmax,
                float prodkm[5],
                string cliente[],
                string producto[],
                string archivo){  
    ofstream save;

    save.open(archivo,ios::app);
    save<<"\nEl cliente con mas tipos de productos que superan los 13000kg es "<<cliente[cmax]<<".\n"
        <<"Los kilometros recorridos por tipo de producto de "<<cliente[cmax]<<" son:\n";
    for (int i=0;i<5;i++){             //copia los km de la matriz a un vector 
        prodkm[i]=km[cmax][i];
    }
    save.close();
    ordenar_vector(prodkm,5);           //ordena el vector
    save.open(archivo,ios::app);
    for(int i=0;i<5;i++){
        int idprod;
        save<<"\t"<<traducir_vec(km,prodkm,i,cmax,producto,idprod)<<": "<<prodkm[i]<<"km.\n";  //busca el producto correspondiente e imprime
    }
}

int cant_prod(int nprod,string archivo){                   //devuelve la cantidad de entregas que tuvo un tipo de producto sin importar el cliente
    ifstream leer;
    leer.open(archivo);
    float nl; //numero leido
    int i=0, entregas=0;
    while(leer>>nl){
        i++;
        switch(i){                          //lee el segundo numero de cada renglon y si es igual al numero de producto que se inserto en la funcion sube en uno la cantidad de entregas
            case 1: break;
            case 2: if(nl==nprod)entregas++;break;
            case 3: break;
            case 4: i=0;break;
        }
    }
    leer.close();
    return entregas;                        //devuelve la cantidad de entregas
}

int main(){
    const int cantidad_clientes=8,Tproductos=5;
    string datos,                        //nombre del archivo donde se encuentran los datos de las entregas.
           datosn,                       //   ||   ||     ||    ||  ||      ||     || nombres de los clientes y los tipos de producto.
           archivo;                      //   ||   ||     ||    ||  || escribirá durante el programa.
    
    
    string cliente[cantidad_clientes],   //lugar donde se cargaran los nombres de los clientes.
           producto[Tproductos];         // ||    ||   ||    ||     ||    ||   ||  || productos.
    
    
    float KG[cantidad_clientes][Tproductos]{}, //matriz en donde se cargará la totalidad de los kg de cada producto por cliente.
          KM[cantidad_clientes][Tproductos]{}; //  ||   ||   ||  ||   ||    ||     ||    ||  || km ||  ||     ||     ||    ||
    
    int idprod;                                //número que se usara para recibir un tipo de producto como un int en vez de string de ser necesario.
    
    ofstream save;
    pickarch(datosn,datos,archivo);                                                 //se eligen los archivos donde estarán los datos.
    carg_dat(KG,KM,datos);                                                          //se cargan los datos 
    carg_nom(cliente,producto,datosn);                                              //se cargan los nombres
#pragma region punto_1
    save.open(archivo,ios::app);
    save<<"Listado por clientes, de los tipos de productos que superen los 13000 Kg acumulados:\n";
    save.close();
    kg13k(KG,cliente,producto,archivo);                             //se llama a "kg13k" que escribe el cliente y los productos>13k
#pragma endregion 
#pragma region punto_2
    float  km_cmax[5];                   //vector en el que se cargaran los kilometros de los productos en orden ascendente;
    list_ascen(KM,maxcliente(KM),km_cmax,//se llama a la funcion que lista ascendentemente los km recorridos con el cliente con mas productos >13k
               cliente,producto,archivo);
#pragma endregion
#pragma region punto_3
    save.open(archivo,ios::app);
    save<<"\nEl producto con menor cantidad de km recorridos es "
        <<traducir_vec(KM,km_cmax,0,maxcliente(KM),producto,idprod)                 //escribe el nombre del producto con menos km del cliente con mas productos>13000kg
        <<" con "<<km_cmax[0]<<"km."                                               //escribe la cantidad de km de km_cmax[0], es decir el producto con menos km
        <<endl<<traducir_vec(KM,km_cmax,0,maxcliente(KM),producto,idprod)           //vuelve a escribir el mismo nombre del producto
        <<" tuvo "<<cant_prod(idprod,datos)<<" entregas entre todos los clientes."; //llama a la funcion que cuenta cuantas entregas tuvo un producto a eleccion
#pragma endregion
    cout<<"Los datos se encuentran en "<<archivo<<"."<<endl
        <<"Presione cualquier tecla para salir.";
    getch();
    return 0;
}