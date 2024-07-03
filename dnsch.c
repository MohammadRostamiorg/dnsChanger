#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>

// Function prototypes
void usage();
double get_average_ping(const char *dns);
void backup_resolv_conf();
void restore_resolv_conf();
void set_nameservers(const char *ns1, const char *ns2);
void clear_nameservers();
void ping_nameservers();

// Struct for passing arguments to threads
typedef struct {
    const char *dns;
    double avg_ping;
} PingResult;

// Main function
int main(int argc, char *argv[]) {
    if (geteuid() != 0) {
        fprintf(stderr, "This script must be run as root\n");
        return 1;
    }

    backup_resolv_conf();

    if (argc < 2) {
        usage();
    }

    if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--set") == 0) {
        if (argc != 4) {
            usage();
        }
        set_nameservers(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--restore") == 0) {
        restore_resolv_conf();
    } else if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--ping") == 0) {
        ping_nameservers();
    } else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--clear") == 0) {
        clear_nameservers();
    } else {
        usage();
    }

    return 0;
}

void usage() {
    fprintf(stderr, "Usage: dnsch {g|sh|ag|cf|403|bg|rd|el} [-c|--clear] [-s|--set] [-p|--ping] [-r|--restore]\n");
    exit(1);
}

double get_average_ping(const char *dns) {
    char command[128];
    snprintf(command, sizeof(command), "ping -c 4 -q %s", dns);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run ping command");
        return 9999.0;
    }

    char result[1024];
    double avg = 9999.0;
    while (fgets(result, sizeof(result), fp) != NULL) {
        if (strstr(result, "rtt min/avg/max/mdev") != NULL) {
            sscanf(result, "rtt min/avg/max/mdev = %*f/%lf/%*f/%*f ms", &avg);
            break;
        }
    }

    pclose(fp);
    return avg;
}

void backup_resolv_conf() {
    struct stat st;
    if (stat("/etc/resolv.conf.bak", &st) != 0) {
        if (errno == ENOENT) {
            FILE *src = fopen("/etc/resolv.conf", "r");
            FILE *dest = fopen("/etc/resolv.conf.bak", "w");

            if (src == NULL || dest == NULL) {
                perror("Failed to backup resolv.conf");
                exit(1);
            }

            char buffer[1024];
            size_t n;
            while ((n = fread(buffer, 1, sizeof(buffer), src)) > 0) {
                if (fwrite(buffer, 1, n, dest) != n) {
                    perror("Failed to write backup file");
                    fclose(src);
                    fclose(dest);
                    exit(1);
                }
            }

            fclose(src);
            fclose(dest);
        } else {
            perror("stat failed");
            exit(1);
        }
    }
}

void restore_resolv_conf() {
    if (rename("/etc/resolv.conf.bak", "/etc/resolv.conf") != 0) {
        perror("Failed to restore resolv.conf");
        exit(1);
    }
    printf("Restored the resolv.conf to what it was before the dnsch.\n");
    exit(0);
}

void set_nameservers(const char *ns1, const char *ns2) {
    FILE *fp = fopen("/etc/resolv.conf.new", "w");
    if (fp == NULL) {
        perror("Failed to open /etc/resolv.conf.new for writing");
        exit(1);
    }

    fprintf(fp, "nameserver %s\n", ns1);
    fprintf(fp, "nameserver %s\n", ns2);
    fclose(fp);

    if (rename("/etc/resolv.conf.new", "/etc/resolv.conf") != 0) {
        perror("Failed to update resolv.conf");
        exit(1);
    }

    printf("The resolv.conf file has been updated:\n");
    system("cat /etc/resolv.conf");
}

void clear_nameservers() {
    FILE *fp = fopen("/etc/resolv.conf.new", "w");
    if (fp == NULL) {
        perror("Failed to open /etc/resolv.conf.new for writing");
        exit(1);
    }
    fclose(fp);

    if (rename("/etc/resolv.conf.new", "/etc/resolv.conf") != 0) {
        perror("Failed to update resolv.conf");
        exit(1);
    }

    printf("Cleared all nameservers in resolv.conf\n");
}

void *ping_thread(void *arg) {
    PingResult *result = (PingResult *)arg;
    result->avg_ping = get_average_ping(result->dns);
    return NULL;
}

void ping_nameservers() {
    const char *dnsList[][3] = {
        {"g", "8.8.8.8", "8.8.4.4"},
        {"sh", "178.22.122.100", "185.51.200.2"},
        {"ag", "94.140.14.14", "94.140.15.15"},
        {"cf", "1.1.1.1", "1.0.0.1"},
        {"403", "10.202.10.202", "10.202.10.102"},
        {"bg", "185.55.226.26", "185.55.225.25"},
        {"rd", "10.202.10.10", "10.202.10.11"},
        {"el", "78.157.42.100", "78.157.42.101"}
    };
    
    int num_dns = sizeof(dnsList) / sizeof(dnsList[0]);
    pthread_t threads[num_dns * 2];
    PingResult results[num_dns * 2];
    
    printf("This command may take a few seconds, please wait...\n");

    for (int i = 0; i < num_dns; i++) {
        results[i * 2].dns = dnsList[i][1];
        results[i * 2 + 1].dns = dnsList[i][2];

        if (pthread_create(&threads[i * 2], NULL, ping_thread, &results[i * 2]) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
        if (pthread_create(&threads[i * 2 + 1], NULL, ping_thread, &results[i * 2 + 1]) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    for (int i = 0; i < num_dns * 2; i++) {
        pthread_join(threads[i], NULL);
    }

    double min_ping = 9999.0;
    const char *best_dns = NULL;

    for (int i = 0; i < num_dns; i++) {
        double avg_ping = (results[i * 2].avg_ping + results[i * 2 + 1].avg_ping) / 2.0;
        printf("%s: %.3f ms\n", dnsList[i][0], avg_ping);

        if (avg_ping < min_ping) {
            min_ping = avg_ping;
            best_dns = dnsList[i][0];
        }
    }

    if (best_dns) {
        printf("\nThe best DNS with the lowest average ping is: %s with %.3f ms\n", best_dns, min_ping);
    }
}
