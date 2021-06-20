SIGHUP  ->1	 -> terminal desligado.
SIGINT  ->2	 -> interrupção do terminal (gerado pela tecla DEL ou Break)
SIGQUIT	->3  -> abortar a execução.
SIGILL  ->4	 -> instrução ilegal.
SIGTRAP ->5  -> Problemas com trace: emitido após cada intrução em caso de geração de traces dos processos (utilização da primitiva ptrace())
SIGABRT ->6  -> abortar processo
SIGFPE	->8  -> Emitido em caso de erro de cálculo em ponto flutuante, assim como no caso de um número em ponto flutuante em formato ilegal. Indica sempre um erro de programação.
SIGKILL ->9  -> mata um processo.
SIGUSR1 ->10 -> sinal para ser usado pelo utilizador.
SIGSEGV ->11 -> Emitido em caso de violação da segmentação: tentativa de acesso a um dado fora do domínio de endereçamento do processo.
SIGUSR2	->12 -> sinal para ser usado pelo utilizador.
SIGPIPE ->13 -> Escrita sobre um pipe não aberto em leitura
SIGALRM	->14 -> manda o sinal SIGALRM (de alarme) a um processo.
SIGTERM ->15 -> terminação ordenada do processo.
SIGCHLD ->17 -> sinal que um processo recebe quando um filho morre (através do exit)
SIGCONT ->18 -> desbloqueia um processo se estiver bloqueado.
SIGSTOP ->19 -> bloqueia um processo.
SIGTSTP ->20 -> for signal - terminal stop
SIGTTIN	->21 -> A process cannot read from the user’s terminal while it is running as a background job. When any process in a background job tries to read from the terminal, all of the processes in the job are sent a SIGTTIN signal. The default action for this signal is to stop the process.
SIGTTOU ->22 -> This is similar to SIGTTIN, but is generated when a process in a background job attempts to write to the terminal or set its modes. Again, the default action is to stop the process. SIGTTOU is only generated for an attempt to write to the terminal if the TOSTOP output mode is set