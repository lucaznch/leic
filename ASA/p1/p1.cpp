#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int slab_dimension_x, slab_dimension_y, number_of_parts_to_produce;

    // read input: slab dimensions and number of parts to produce
    scanf("%d %d", &slab_dimension_x, &slab_dimension_y);
    scanf("%d", &number_of_parts_to_produce);


    // 2D vector with dimension (slab_dim_x + 1) x (slab_dim_y + 2) and all values are init to 0
    vector<vector<int>> dp(slab_dimension_x + 1, vector<int>(slab_dimension_y + 1, 0));


    // read the rest of input:  parts to produce for clients
    for (int i = 0; i < number_of_parts_to_produce; ++i) {
        int ai, bi, pi;
        scanf("%d %d %d", &ai, &bi, &pi);

        // check if the part can fit without rotation
        if (ai <= slab_dimension_x && bi <= slab_dimension_y) {
            dp[ai][bi] = max(dp[ai][bi], pi);
        }

        // check if the part can fit with rotation
        if (ai <= slab_dimension_y && bi <= slab_dimension_x) {
            dp[bi][ai] = max(dp[bi][ai], pi);
        }
    }

    // dynamic programming
    for (int i = 1; i <= slab_dimension_x; ++i) {
        for (int j = 1; j <= slab_dimension_y; ++j) {
            for (int k = 1; k < i; ++k) {
                dp[i][j] = max(dp[i][j], dp[k][j] + dp[i - k][j]);
            }
            for (int k = 1; k < j; ++k) {
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[i][j - k]);
            }
        }
    }

    printf("%d\n", dp[slab_dimension_x][slab_dimension_y]); // print the maximum value

    return 0;
}
