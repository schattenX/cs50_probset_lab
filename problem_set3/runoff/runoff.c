#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();                           // 寻找票数最少的
        bool tie = is_tie(min);                         // 判断所有人票数都是最少且相同

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);                                 // 若存在n个竞选者满足 N.votes == min && 0 < N(N.votes=min) < candidate_count

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    // 不实时更新被投票者对应的票数
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)  // 遍历结构体数组找到被投票者所在结构体数组内的下标
        {
            preferences[voter][rank] = i;           // preferences记录第voter个选民的第rank个评级所竞选的对象，其在candidates数组中
            return true;                            // 所对应相同竞选对象名称的下标或索引
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    int Ranks[voter_count];                          // 记录用于计算票数的voter偏好下标，rank[i]默认初始化都为零
    for (int i = 0; i < voter_count; i++)           // rank[i] = 0表示第i+1个voter的第一个偏好，其他的以此类推
    {
        int rank = Ranks[i];
        if (!candidates[preferences[i][rank]].eliminated)
        {
            candidates[preferences[i][rank]].votes++;
        }
        // 若存在有竞选者被淘汰
        else
        {
            rank = ++Ranks[i];                      // 有竞选者被淘汰，则第i+1个voter默认偏好置为次偏好

            candidates[preferences[i][rank]].votes++;
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    // voter_count除以2四舍五入
    // int major_voter = ((int)(voter_count / 2.0 * 10.0) % 10 >= 5) ? (voter_count / 2 + 1) : (voter_count / 2);
    int major_voter = voter_count / 2;          // 无需四舍五入
    // 用以最后判断是否存在（至少有一个winner）
    int flag = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        // 既是没有被淘汰的，又是票数大于等于一半的投票者以上的竞选者被打印
        if (!candidates[i].eliminated && candidates[i].votes > major_voter) // 这里大于等于换成大于即可
        {
            printf("%s\n", candidates[i].name);
            flag = 1;
        }
    }

    if (flag == 1) return true;

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int min = candidates[0].votes;
    for (int i = 1; i < candidate_count; i++)
    {
        // 竞选者参与最少票数计算，当且仅当没有被淘汰且票数比预设min还小
        if (!candidates[i].eliminated && min > candidates[i].votes)
        {
            min = candidates[i].votes;
        }
    }

    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    // 与最小值min相等的总人数
    int pnum_equal_min = 0;
    int remaining_pnum = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            remaining_pnum++;
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            pnum_equal_min++;
        }
    }
    // 若余下的人中，每个竞选者票数都相同且都为min，则返回true
    if (pnum_equal_min == remaining_pnum) return true;
    // 
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    int temp = candidate_count;
    for (int i = 0; i < temp; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
           // candidate_count--;        无需此行，只需要做形式的删除eliminated = 1；否则120~123行会出现逻辑错误
        }
    }

    return;
}