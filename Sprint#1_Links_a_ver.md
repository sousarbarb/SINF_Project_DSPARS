Neste ficheiro está presente uma série de links a ver para efetuar-se a programação da aplicação em C no Sprint#1:
* Como executar comandos no sistema a partir de um programa em C:
  - https://www.tutorialspoint.com/c_standard_library/c_function_system.htm
* Uso da função POPEN para leitura e escrita de dados na consola:
  - https://stackoverflow.com/questions/16127027/linux-command-executing-by-popen-on-c-code
  - http://c-for-dummies.com/blog/?p=1418  _(explicação da função)_
  - http://pubs.opengroup.org/onlinepubs/009696699/functions/popen.html
* Uso de Funções em PROCESSOS:
  - fork:
    - https://stackoverflow.com/questions/32810981/fork-function-in-c
    - http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html
    - https://www.geeksforgeeks.org/fork-system-call/
    - http://man7.org/linux/man-pages/man2/fork.2.html
    - https://en.wikipedia.org/wiki/Fork_(system_call)
  - wait:
    - http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/wait.html
    - http://man7.org/linux/man-pages/man2/waitpid.2.html
    - https://stackoverflow.com/questions/13216554/what-does-wait-do-on-unix
    - https://www.geeksforgeeks.org/wait-system-call-c/
    - http://pubs.opengroup.org/onlinepubs/009695399/functions/wait.html
    - https://en.wikipedia.org/wiki/Wait_(system_call)
  - O tipo de variáveis associadas aos processos é pid_t
* Uso de Funções em THREADS:
  - pthread_create:
    - http://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_create.html
    - http://man7.org/linux/man-pages/man3/pthread_create.3.html
    - https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/ptcrea.htm
    - https://stackoverflow.com/questions/6990888/c-how-to-create-thread-using-pthread-create-function
  - pthread_join:
    - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
    - http://man7.org/linux/man-pages/man3/pthread_join.3.html
    - https://stackoverflow.com/questions/8513894/pthread-join-and-pthread-exit
    - https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/ptjoin.htm
  - pthread_exit:
    - https://stackoverflow.com/questions/20824229/when-to-use-pthread-exit-and-when-to-use-pthread-join-in-linux
    - http://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_exit.html
    - https://www.thegeekstuff.com/2012/04/terminate-c-thread/
    - https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/ptexit.htm
    - http://man7.org/linux/man-pages/man3/pthread_exit.3.html
