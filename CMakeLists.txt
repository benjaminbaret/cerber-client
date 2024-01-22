# CMakeLists.txt à la racine du projet

# Déclaration du projet principal
cmake_minimum_required(VERSION 3.12)
project(cerber-client)

# Add
add_subdirectory(src)

# Ajout du dossier "test" pour l'exécutable de test
add_subdirectory(test)
