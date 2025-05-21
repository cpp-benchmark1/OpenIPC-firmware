#!/bin/bash

# Atualiza os dados remotos
git fetch --all

# Lista todas as branches remotas, excluindo HEAD e origin/HEAD
for branch in $(git branch -r | grep -vE 'HEAD|origin/HEAD'); do
    # Remove o prefixo "origin/" para obter o nome da branch local
    local_branch=${branch#origin/}
    
    # Cria a branch local acompanhando a remota
    git branch "$local_branch" --track "$branch"
    
    echo "Branch local '$local_branch' criada a partir de '$branch'"
done
