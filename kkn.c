#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
int arr[150] = {0};
#define MAX_SIZE_OF_TotalSET 150
#define MAX_SIZE_OF_TRAINING_SET 120
#define MAX_SIZE_OF_TEST_SET 30
struct dataVector
{
    char *classLabel;
    char *attributes[4];
};
struct distanceStruct
{
    double distance;
    char *classLabel;
};
struct dataVector totalset[MAX_SIZE_OF_TotalSET];
struct dataVector TrainingSet[MAX_SIZE_OF_TRAINING_SET];
struct dataVector TestSet[MAX_SIZE_OF_TEST_SET];
int K = 3;
struct distanceStruct NearestDistance[3];
double Distance(struct dataVector p1, struct dataVector p2)
{
    double dist, sum = 0.0;
    for (int i = 0; i < 4; i++)
    {
        double x1 = atoi(p1.attributes[i]);
        double x2 = atoi(p2.attributes[i]);
        sum += (x1 - x2) * (x1 - x2);
    }
    dist = sqrt(sum);
    return dist;
}
int GetMaxDistance(int K)
{
    int maxNo = 0;
    for (int i = 1; i < K; i++)
    {
        if (NearestDistance[i].distance > NearestDistance[maxNo].distance)
            maxNo = i;
    }
    return maxNo;
}
char *Classify(struct dataVector Sample, int size, int K)
{
    double dist = 0;
    int maxid = 0, freq[K], i, tmpfreq = 1;
    char *curClassLable = NearestDistance[0].classLabel;
    memset(freq, 1, sizeof(freq));
    for (i = 0; i < K; i++)
        NearestDistance[i].distance = 10000.0;
    for (i = 0; i < size; i++)
    {
        dist = Distance(TrainingSet[i], Sample);
        maxid = GetMaxDistance(K);
        if (dist < NearestDistance[maxid].distance)
        {
            NearestDistance[maxid].distance = dist;
            NearestDistance[maxid].classLabel = TrainingSet[i].classLabel;
        }
    }
    for (i = 0; i < K; i++)
    {
        for (int j = 0; j < K; j++)
        {
            if ((i != j) && (NearestDistance[i].classLabel == NearestDistance[j].classLabel))
            {
                freq[i] += 1;
            }
        }
    }
    for (i = 0; i < K; i++)
    {
        if (freq[i] > tmpfreq)
        {
            tmpfreq = freq[i];
            curClassLable = NearestDistance[i].classLabel;
        }
    }
    return curClassLable;
}
// 5 fold cross validation
int k_fold(int k_kkn)
{
    int no_ts = 4;
    int m = 120 / 5, i = 0, start = 0, j = m;
    while (no_ts != 0)
    {
        int count = 0, t_size, tr_size = 0;
        int y = 0;
        for (i = start; i < j; i++)
        {
            TestSet[y].attributes[0] = totalset[i].attributes[0];
            TestSet[y].attributes[1] = totalset[i].attributes[1];
            TestSet[y].attributes[2] = totalset[i].attributes[2];
            TestSet[y].attributes[3] = totalset[i].attributes[3];
            TestSet[y].classLabel = totalset[i].classLabel;
            count++;
            y++;
        }
        t_size = m;
        tr_size = m * 4;
        int store = i;
        y = 0;
        if ((i - m) != 0)
        {
            for (int x = 0; x < i; x++)
            {
                TrainingSet[y].attributes[0] = totalset[x].attributes[0];
                TrainingSet[y].attributes[1] = totalset[x].attributes[1];
                TrainingSet[y].attributes[2] = totalset[x].attributes[2];
                TrainingSet[y].attributes[3] = totalset[x].attributes[3];
                TrainingSet[y].classLabel = totalset[x].classLabel;
                y++;
            }
        }
        if (i + m < m * 4)
        {
            for (i; i < m * 4; i++)
            {
                TrainingSet[y].attributes[0] = totalset[i].attributes[0];
                TrainingSet[y].attributes[1] = totalset[i].attributes[1];
                TrainingSet[y].attributes[2] = totalset[i].attributes[2];
                TrainingSet[y].attributes[3] = totalset[i].attributes[3];
                TrainingSet[y].classLabel = totalset[i].classLabel;
                y++;
            }
        }
        int Ctrue = 0;
        for (int p = 0; p < t_size; p++)
        {
            char *label = Classify(TestSet[p], tr_size, k_kkn);
            if (atoi(label) == atoi(TestSet[p].classLabel))
                Ctrue++;
        }
        printf("\n first one k is %d == %d  \n", k_kkn, (Ctrue * 100) / 30);
        start += m;
        no_ts--;
    }
    return 1;
}

int main()
{
    FILE *fp = fopen("data.txt", "r");
    if (fp == NULL)
    {
        perror("unable to open the file");
        exit(1);
    }
    char line[50];
    srand(time(NULL));
    int count = 0;
    int prev;
    while (fgets(line, sizeof(line), fp))
    {
        count++;
        int got = 0, i;
        while (got != 1)
        {
            i = rand() % 149;
            if (arr[i] == 0)
            {
                arr[i] = 1;
                got = 1;
            }
        }
        char *token = strtok(line, ",");
        int j = 0;
        while (token != 0)
        {
            if (j == 3)
            {
                char *extra = strtok(token, " ");
                while (extra != 0)
                {
                    totalset[i].attributes[3] = strdup(extra);
                    extra = strtok(NULL, " ");
                    totalset[i].classLabel = strdup(extra);
                    break;
                }
                break;
            }
            totalset[i].attributes[j++] = strdup(token);
            token = strtok(NULL, ",");
        }
        if (count == 149)
            fclose(fp);
    }

    for (int i = 0; i < 120; i++)
    {
        TrainingSet[i].attributes[0] = totalset[i].attributes[0];
        TrainingSet[i].attributes[1] = totalset[i].attributes[1];
        TrainingSet[i].attributes[2] = totalset[i].attributes[2];
        TrainingSet[i].attributes[3] = totalset[i].attributes[3];
        TrainingSet[i].classLabel = totalset[i].classLabel;
    }
    int j = 0;
    int kf = 5;
    int cv = k_fold(kf);
    printf("\nstart");
    k_fold(5);
    printf("end\n");
    return 0;
}