from pulp import LpProblem, LpMaximize, LpVariable, lpSum, GLPK

# Reading input
t, p, max_toys = map(int, input().split())
toy_data = [tuple(map(int, input().split())) for _ in range(t)]
package_data = [tuple(map(int, input().split())) for _ in range(p)]

# Filter bundles based on the profit condition
valid_bundles = [(i, j, k, lijk) for i, j, k, lijk in package_data
                 if lijk > toy_data[i-1][0] + toy_data[j-1][0] + toy_data[k-1][0]]

# Create LP problem
prob = LpProblem("ToysInc_MaxProfit", LpMaximize)

# Decision Variables
x = [LpVariable(f"x_{i}", lowBound=0, cat='Integer') for i in range(t)]
y = [LpVariable(f"y_{i}_{j}_{k}", lowBound=0, cat='Integer') for i, j, k, _ in valid_bundles]

# Constraints
for i in range(t):
    prob += x[i] <= toy_data[i][1]


for i, j, k, _ in valid_bundles:
    index = valid_bundles.index((i, j, k, _))
    prob += y[index] <= min(toy_data[i-1][1], toy_data[j-1][1], toy_data[k-1][1])
    prob += 3 * y[index] + x[i-1] + x[j-1] + x[k-1] <= toy_data[i-1][1] + toy_data[j-1][1] + toy_data[k-1][1]

prob += lpSum(x) + 3 * lpSum(y) <= max_toys

# Objective Function
profit_expr = lpSum(toy_data[i-1][0] * x[i-1] for i in range(1, t+1))
profit_expr += lpSum(valid_bundles[i-1][3] * y[i-1] for i in range(1, len(valid_bundles)+1))

prob += profit_expr

# Solve the problem
prob.solve(GLPK(msg=0))

# Output the result
print(int(prob.objective.value()))
