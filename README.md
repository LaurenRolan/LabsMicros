# LabsMicros
Laboratórios de Microcontroladores - 2018/1

---

##Comandos básicos
* Adição de usuário
  useradd -c "Nome por Extenso" -s /bin/bash -m <login>
  userdel -r <login> #caso login tenha sido criado errado
  passwd <login>
  exit
  ssh <login>@<galileoname>
  echo "export PATH=.:$PATH" >> .profile
  exit
  
* Transferência de arquivos
  scp <arquivo_no_host> <login>@<galileoname>:
  scp <login>@<galileoname>:<arquivo_na_galileo> <path_no_host>
  
* Depuração remota
  
