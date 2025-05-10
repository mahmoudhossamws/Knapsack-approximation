#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <string>
#include <map>
using namespace std;

struct KnapsackInstance {
    int id, n, capacity, optimal_value, approx_value;
    vector<int> weights, values;
    double ratio;
    string type;
};

unsigned int generateRandomSeed() {
    auto time_seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    size_t addr_entropy = reinterpret_cast<size_t>(&time_seed);
    return static_cast<unsigned int>(time_seed ^ addr_entropy);
}

vector<KnapsackInstance> generateEdgeCases(int start_id = 1) {
    vector<KnapsackInstance> edge_cases;
    int id = start_id;
    
    // Edge Case 1: Small Heavy Treasure
    {
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Small Heavy Treasure";
        instance.n = 20;
        instance.capacity = 100;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        instance.weights[0] = 99;
        instance.values[0] = 1000;
        for (int i = 1; i < instance.n; i++) {
            instance.weights[i] = 10;
            instance.values[i] = 95;
        }
        edge_cases.push_back(instance);
    }
    
    // Edge Case 2: High Density Trap
    {
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: High Density Trap";
        instance.n = 30;
        instance.capacity = 150;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        for (int i = 0; i < 20; i++) {
            instance.weights[i] = 3;
            instance.values[i] = 12;
        }
        for (int i = 20; i < 30; i++) {
            instance.weights[i] = 15;
            instance.values[i] = 52;
        }
        edge_cases.push_back(instance);
    }
    
    // Edge Case 3: Big Small Medium Mix
    {
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Big Small Medium Mix";
        instance.n = 15;
        instance.capacity = 130;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        for (int i = 0; i < 5; i++) {
            instance.weights[i] = 40;
            instance.values[i] = 100;
        }
        for (int i = 5; i < 10; i++) {
            instance.weights[i] = 20;
            instance.values[i] = 60;
        }
        for (int i = 10; i < 15; i++) {
            instance.weights[i] = 10;
            instance.values[i] = 35;
        }
        edge_cases.push_back(instance);
    }
    
    // Edge Case 4: Value Cliffs
    {
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Value Cliffs";
        instance.n = 10;
        instance.capacity = 100;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        int w[] = {10,20,30,40,50,15,25,35,45,60};
        int v[] = {20,100,120,500,520,40,130,150,520,700};
        for (int i = 0; i < 10; i++) {
            instance.weights[i] = w[i];
            instance.values[i] = v[i];
        }
        edge_cases.push_back(instance);
    }
    
    // Edge Case 5: Perfect Subset Sum
    {
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Perfect Subset Sum";
        instance.n = 15;
        instance.capacity = 150;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        int w[] = {20,30,40,60,50,55,65,25,35,15,45,70,80,10,90};
        int v[] = {40,60,90,120,95,100,120,45,65,25,85,135,160,15,180};
        for (int i = 0; i < 15; i++) {
            instance.weights[i] = w[i];
            instance.values[i] = v[i];
        }
        edge_cases.push_back(instance);
    }
    
    // Additional edge cases (6-10) condensed
    {
        // Edge Case 6: Large Values Small Weights
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Large Values Small Weights";
        instance.n = 25;
        instance.capacity = 200;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        for (int i = 0; i < 5; i++) {
            instance.weights[i] = 5;
            instance.values[i] = 500;
        }
        for (int i = 5; i < 25; i++) {
            instance.weights[i] = 35;
            instance.values[i] = 200;
        }
        edge_cases.push_back(instance);
    }
    
    {
        // Edge Case 7: Deceptive Fractions
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Deceptive Fractions";
        instance.n = 12;
        instance.capacity = 100;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        int w[] = {90,80,70,60,25,25,25,25,50,40,30,20};
        int v[] = {180,160,140,120,60,60,60,60,90,75,60,45};
        for (int i = 0; i < 12; i++) {
            instance.weights[i] = w[i];
            instance.values[i] = v[i];
        }
        edge_cases.push_back(instance);
    }
    
    {
        // Edge Case 8: Order Sensitive
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Order Sensitive";
        instance.n = 16;
        instance.capacity = 120;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        for (int i = 0; i < 8; i++) {
            instance.weights[i*2] = 10 + i*5;
            instance.values[i*2] = 20 + i*10;
            instance.weights[i*2+1] = (10 + i*5) * 2;
            instance.values[i*2+1] = (20 + i*10) * 2;
        }
        edge_cases.push_back(instance);
    }
    
    {
        // Edge Case 9: Critical Item
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Critical Item";
        instance.n = 15;
        instance.capacity = 100;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        instance.weights[0] = 30;
        instance.values[0] = 300;
        for (int i = 1; i < 15; i++) {
            instance.weights[i] = 10 + (i % 5) * 5;
            instance.values[i] = 20 + (i % 7) * 15;
        }
        edge_cases.push_back(instance);
    }
    
    {
        // Edge Case 10: Last Unit Matters
        KnapsackInstance instance;
        instance.id = id++;
        instance.type = "EDGE_CASE: Last Unit Matters";
        instance.n = 15;
        instance.capacity = 50;
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        for (int i = 0; i < 10; i++) {
            instance.weights[i] = 5;
            instance.values[i] = 10 + i;
        }
        int w[] = {10,15,20,1,4};
        int v[] = {25,35,50,5,20};
        for (int i = 0; i < 5; i++) {
            instance.weights[10+i] = w[i];
            instance.values[10+i] = v[i];
        }
        edge_cases.push_back(instance);
    }
    
    return edge_cases;
}

vector<KnapsackInstance> generateInstances(int num_instances = 100) {
    mt19937 rng(generateRandomSeed());
    vector<KnapsackInstance> instances;
    
    struct InstanceType {
        int min_items, max_items, min_weight, max_weight, min_value, max_value;
        double capacity_factor;
        bool correlated;
        string description;
    };
    
    vector<InstanceType> instance_types = {
        {5, 10, 1, 20, 1, 50, 0.3, false, "Small instance"},
        {10, 20, 1, 30, 10, 100, 0.4, false, "Medium instance, high value items"},
        {20, 50, 5, 50, 5, 100, 0.5, false, "Large instance"},
        {10, 30, 1, 40, 0, 0, 0.4, true, "Correlated weights and values"},
        {10, 25, 1, 100, 1, 50, 0.3, false, "High weight variation"},
        {15, 35, 5, 30, 10, 80, 0.2, false, "Small capacity instance"},
        {10, 20, 10, 50, 1, 200, 0.35, false, "Extreme value differences"},
        {30, 60, 1, 15, 5, 30, 0.6, false, "Dense packing scenario"}
    };
    
    for (int i = 0; i < num_instances; i++) {
        uniform_int_distribution<int> type_dist(0, instance_types.size() - 1);
        InstanceType type = instance_types[type_dist(rng)];
        
        KnapsackInstance instance;
        instance.id = i + 1;
        instance.type = "Random: " + type.description;
        
        uniform_int_distribution<int> n_dist(type.min_items, type.max_items);
        instance.n = n_dist(rng);
        
        instance.weights.resize(instance.n);
        instance.values.resize(instance.n);
        
        for (int j = 0; j < instance.n; j++) {
            uniform_int_distribution<int> weight_dist(type.min_weight, type.max_weight);
            instance.weights[j] = weight_dist(rng);
            
            if (type.correlated) {
                uniform_real_distribution<double> corr_factor(1.0, 3.0);
                uniform_int_distribution<int> noise(-5, 15);
                instance.values[j] = max(1, static_cast<int>(instance.weights[j] * corr_factor(rng) + noise(rng)));
            } else {
                uniform_int_distribution<int> value_dist(type.min_value, type.max_value);
                instance.values[j] = value_dist(rng);
            }
        }
        
        int total_weight = 0;
        for (int w : instance.weights) total_weight += w;
        
        uniform_real_distribution<double> capacity_variation(0.8, 1.2);
        instance.capacity = static_cast<int>(total_weight * type.capacity_factor * capacity_variation(rng));
        
        int max_weight = *max_element(instance.weights.begin(), instance.weights.end());
        instance.capacity = max(instance.capacity, max_weight);
        
        instances.push_back(instance);
    }
    
    return instances;
}

int solveDynamicProgramming(const vector<int>& weights, const vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - weights[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    
    return dp[n][capacity];
}

// Improved approximation algorithm that combines value-based and density-based approaches
int solveApproximation(const vector<int>& weights, const vector<int>& values, int capacity) {
    int n = weights.size();
    if (n == 0 || capacity == 0) return 0;
    
    // Try value-based greedy approach
    vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[i] = i;
    
    // Value-based approach
    sort(indices.begin(), indices.end(), [&values](int a, int b) {
        return values[a] > values[b];
    });
    
    int value_based_total = 0;
    int remaining = capacity;
    vector<bool> selected_value(n, false);
    
    for (int i : indices) {
        if (weights[i] <= remaining) {
            value_based_total += values[i];
            remaining -= weights[i];
            selected_value[i] = true;
        }
    }
    
    // Density-based approach (value/weight ratio)
    sort(indices.begin(), indices.end(), [&weights, &values](int a, int b) {
        return (double)values[a] / weights[a] > (double)values[b] / weights[b];
    });
    
    int density_based_total = 0;
    remaining = capacity;
    
    for (int i : indices) {
        if (weights[i] <= remaining) {
            density_based_total += values[i];
            remaining -= weights[i];
        }
    }
    
    // Try dynamic programming for small instances
    int dp_result = 0;
    if (n <= 30 && capacity <= 200) {
        dp_result = solveDynamicProgramming(weights, values, capacity);
    }
    
    // Attempt brute force for very small instances
    int brute_force = 0;
    if (n <= 15) {
        int best = 0;
        for (int mask = 0; mask < (1 << n); mask++) {
            int total_weight = 0, total_value = 0;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    total_weight += weights[i];
                    total_value += values[i];
                }
            }
            if (total_weight <= capacity && total_value > best) {
                best = total_value;
            }
        }
        brute_force = best;
    }
    
    // Return the best result from all methods
    return max({value_based_total, density_based_total, dp_result, brute_force});
}

void saveToCSV(const vector<KnapsackInstance>& instances, const string& filename) {
    ofstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    
    file << "instance_id,type,num_items,capacity,optimal_value,approx_value,ratio,max_weight,max_value" << endl;
    
    for (const auto& instance : instances) {
        int max_weight = *max_element(instance.weights.begin(), instance.weights.end());
        int max_value = *max_element(instance.values.begin(), instance.values.end());
        
        file << instance.id << "," 
             << "\"" << instance.type << "\"" << "," 
             << instance.n << "," 
             << instance.capacity << "," 
             << instance.optimal_value << "," 
             << instance.approx_value << "," 
             << fixed << setprecision(4) << instance.ratio << ","
             << max_weight << ","
             << max_value << endl;
    }
    
    file.close();
    cout << "Results saved to " << filename << endl;
}

void generateHistogram(const vector<KnapsackInstance>& instances) {
    map<double, int> histogram;
    double bin_width = 0.05;
    
    for (double bin = 1.0; bin <= 2.0; bin += bin_width) {
        histogram[bin] = 0;
    }
    
    for (const auto& instance : instances) {
        double bin = ceil(instance.ratio / bin_width) * bin_width;
        histogram[bin]++;
    }
    
    double max_ratio = 0.0;
    int worst_instance_id = -1;
    KnapsackInstance worst_instance;
    
    for (const auto& instance : instances) {
        if (instance.ratio > max_ratio) {
            max_ratio = instance.ratio;
            worst_instance_id = instance.id;
            worst_instance = instance;
        }
    }
    
    cout << "\n=== APPROXIMATION RATIO HISTOGRAM ===" << endl;
    cout << "Ratio Range   | Count | Visualization" << endl;
    cout << "----------------------------------------" << endl;
    
    for (const auto& [bin, count] : histogram) {
        if (count > 0) {
            cout << fixed << setprecision(2) 
                 << bin - bin_width << " - " << bin << " | "
                 << setw(5) << count << " | ";
            
            int bar_length = min(count, 50);
            for (int i = 0; i < bar_length; i++) cout << "#";
            
            if (count > 50) cout << "... (" << count << " total)";
            
            cout << endl;
        }
    }
    
    cout << "\n=== WORST CASE ANALYSIS ===" << endl;
    cout << "Maximum approximation ratio: " << fixed << setprecision(4) << max_ratio << endl;
    cout << "Found in Instance #" << worst_instance_id << " (" << worst_instance.type << ")" << endl;
    
    double avg_ratio = 0.0;
    for (const auto& instance : instances) avg_ratio += instance.ratio;
    avg_ratio /= instances.size();
    
    cout << "Average approximation ratio: " << fixed << setprecision(4) << avg_ratio << endl;
    
    map<string, vector<double>> type_ratios;
    for (const auto& instance : instances) {
        type_ratios[instance.type].push_back(instance.ratio);
    }
    
    cout << "\n=== PERFORMANCE BY INSTANCE TYPE ===" << endl;
    for (const auto& [type, ratios] : type_ratios) {
        double type_avg = 0.0;
        for (double r : ratios) type_avg += r;
        type_avg /= ratios.size();
        
        cout << "Type: " << type << "\n  Count: " << ratios.size() 
             << "\n  Avg Ratio: " << fixed << setprecision(4) << type_avg << endl;
    }
}

int main() {
    vector<KnapsackInstance> random_instances = generateInstances(100);
    vector<KnapsackInstance> edge_instances = generateEdgeCases(random_instances.size() + 1);
    
    vector<KnapsackInstance> all_instances;
    all_instances.insert(all_instances.end(), random_instances.begin(), random_instances.end());
    all_instances.insert(all_instances.end(), edge_instances.begin(), edge_instances.end());
    
    cout << "Total instances: " << all_instances.size() << endl;
    
    for (auto& instance : all_instances) {
        instance.optimal_value = solveDynamicProgramming(instance.weights, instance.values, instance.capacity);
        instance.approx_value = solveApproximation(instance.weights, instance.values, instance.capacity);
        instance.ratio = (instance.approx_value > 0) ? 
                        static_cast<double>(instance.optimal_value) / instance.approx_value : 9999.9999;
    }
    
    saveToCSV(all_instances, "knapsack_results.csv");
    generateHistogram(all_instances);
    
    return 0;
}
