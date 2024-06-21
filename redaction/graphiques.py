import os
import pandas as pd
import matplotlib.pyplot as plt
import sys



# Définition des chemins depuis la racine 
path_csv = './out/'
path_graphs = './redaction/graphiques/'

# Fonction pour tracer les intervalles de confiance
def draw(df, title, path_file, file_name):
    days = range(len(df))
    plt.figure(figsize=(10, 6))
    plt.fill_between(days, df['Borne_inférieure'], df['Borne_supérieure'], color='skyblue', alpha=0.5)
    plt.plot(days, df['Borne_inférieure'], label='Borne inférieure', color='blue')
    plt.plot(days, df['Borne_supérieure'], label='Borne supérieure', color='blue')
    plt.xlabel('Jour')
    plt.ylabel('Nombre de contaminations')
    plt.title(title)
    plt.legend()
    plt.grid(True)
    # Enregistrer le graphique dans le répertoire "graphiques"
    plt.savefig(os.path.join(path_file, file_name))
    plt.close()


if __name__ == "__main__":

    if len(sys.argv) > 1:
        subpath_exp = sys.argv[1]

    else :
        subpath_exp = ''

    # Créer le répertoire s'il n'existe pas
    os.makedirs(path_graphs + subpath_exp, exist_ok=True)


    for i in range(1,10+1):

        str_i = str(i)
        title = 'Scénario ' + str_i + ': Initialisation avec ' + str_i + ' personne contaminée'
        file_name = 'num_infect_init_' + str_i

        # Charger le fichier CSV
        df = pd.read_csv(path_csv + file_name + '.csv')

        # Renommer les colonnes pour plus de clarté
        df.columns = ['Borne_inférieure', 'Borne_supérieure']
        
        # Tracer et enregistrer les graphiques pour chaque scénario
        draw(df, title, path_graphs + subpath_exp, file_name + '.png')
