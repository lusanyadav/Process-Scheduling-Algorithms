#include<iostream>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<climits>
using namespace std;

class process{
public:
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rt;
    int priority;
    int remaining_time;
    int bt_remaining;
    int start_time;
};
vector<process> ps;

bool cmp(class process &c, class process &d){
    return c.pid < d.pid;
}

bool lambda (class process &a, class process &b){
    return a.at < b.at;
}




void fcfs(int n){


    float sum_tat = 0.0, sum_wt = 0.0, sum_rt = 0.0;
    int total_cycle_length = 0;
    int total_idle_time = 0;
    float cpu_utilization = 0.0;

    cout << fixed << setprecision(2)<<endl;
    sort(ps.begin(), ps.end(), lambda);

    for (int i = 0; i < n; i++){
        ps[i].start_time = (i == 0) ? ps[i].at : max(ps[i].at, ps[i - 1].ct);
        ps[i].ct = ps[i].start_time + ps[i].bt;
        ps[i].tat = ps[i].ct - ps[i].at;
        ps[i].wt = ps[i].tat - ps[i].bt;
        ps[i].rt = ps[i].wt;

        sum_tat += ps[i].tat;
        sum_wt += ps[i].wt;
        sum_rt += ps[i].rt;

        total_idle_time += (i == 0) ? 0 : ps[i].start_time - ps[i - 1].ct;
    }
    total_cycle_length = ps[n - 1].ct - ps[0].start_time;



    sort(ps.begin(), ps.end(), cmp);



    cout << "\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
    for(int i=0;i<n;i++)
     cout<<i<<"\t\t"<<ps[i].at<<"\t"<<ps[i].bt<<"\t\t"<<ps[i].ct<<"\t"<<ps[i].tat<<"\t"<<ps[i].wt<<"\t"<<ps[i].rt<<endl;
    cout<<endl;
    
    cpu_utilization = (float)(total_cycle_length - total_idle_time)/ total_cycle_length;
    
    cout<<"\nAverage Turn Around time= "<< sum_tat/n;
    cout<<"\nAverage Waiting Time= "<<sum_wt/n;
    cout<<"\nAverage Response Time= "<<sum_rt/n;    
    cout<<"\nThroughput= "<<n/(float)total_cycle_length;
    cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;

    cout << endl;
}

void sjf_non_preemptive(int n){


    vector<bool> is_completed(n, false);
    bool is_first = true;
    int current_time = 0;
    int completed = 0;
    int prev_process_completion_time = 0;
    float sum_tat = 0.0, sum_wt = 0.0, sum_rt = 0.0;
    int total_cycle_length = 0;
    int total_idle_time = 0;
    float cpu_utilization = 0.0;

    int max_completion_time = 0;
    int min_arrival_time = 0;

    cout << fixed << setprecision(2);

    while(completed != n){

        //find process with min. burst time in ready queue at current time
        int min_index = -1;
        int minimum = INT_MAX;
        for (int i = 0; i < n; i++){
            if(ps[i].at <= current_time && is_completed[i] == false){
                if(ps[i].bt < minimum){
                    min_index = i;
                    minimum = ps[i].bt;
                }
                if(ps[i].bt == minimum){
                    if(ps[i].at < ps[min_index].at){
                        min_index = i;
                        minimum = ps[i].bt;
                    }
                }
            }
        }

        if(min_index == -1){
            current_time++;
        }
        else{
            ps[min_index].start_time = current_time;
            ps[min_index].ct = ps[min_index].start_time + ps[min_index].bt;
            ps[min_index].tat = ps[min_index].ct - ps[min_index].at;
            ps[min_index].wt = ps[min_index].tat - ps[min_index].bt;
            ps[min_index].rt = ps[min_index].wt;

            sum_tat += ps[min_index].tat;
            sum_wt += ps[min_index].wt;
            sum_rt += ps[min_index].wt;

            total_idle_time += (is_first == true) ? 0 : (ps[min_index].start_time - prev_process_completion_time);

            completed++;
            is_first = false;
            is_completed[min_index] = true;

            current_time = ps[min_index].ct;
            prev_process_completion_time = current_time;
        }
    }

     //Calculate Length of Process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;

    for (int i = 0; i < n; i++){
        min_arrival_time = min(min_arrival_time, ps[i].at);
        max_completion_time = max(max_completion_time, ps[i].ct);
    }

    total_cycle_length = max_completion_time - min_arrival_time;


    //Output

    cout<<"\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
    for(int i=0;i<n;i++)
     cout<<i<<"\t\t"<<ps[i].at<<"\t"<<ps[i].bt<<"\t\t"<<ps[i].ct<<"\t"<<ps[i].tat<<"\t"<<ps[i].wt<<"\t"<<ps[i].rt<<endl;
    cout<<endl;

    cpu_utilization = (float)(total_cycle_length - total_idle_time) / total_cycle_length;

    cout<<"\nAverage Turn Around time= "<< (float)sum_tat/n;
    cout<<"\nAverage Waiting Time= "<<(float)sum_wt/n;
    cout<<"\nAverage Response Time= "<<(float)sum_rt/n;    
    cout<<"\nThroughput= "<<n/(float)total_cycle_length;
    cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;

    cout << endl;
    cout << endl;
}




void sjf_preemptive(int n){


    float sum_tat = 0.0, sum_wt = 0.0, sum_rt = 0.0;
    float total_cycle_length = 0;
    float total_idle_time = 0;
    float cpu_utilization = 0.0;

    vector<int> bt_remaining(n, 0);
    vector<bool> is_completed(n, false);
    bool is_first = true;
    int current_time = 0;
    int completed = 0;
    int prev_process_completion_time = 0;

    int max_completion_time = 0;
    int min_arrival_time = 0;

    cout << fixed << setprecision(2);

    for (int i = 0; i < n; i++){
        bt_remaining[i] = ps[i].bt;
    }

        while (completed != n)
        {
            int min_index = -1;
            int minimum = INT_MAX;
            for (int i = 0; i < n; i++)
            {
                if (ps[i].at <= current_time && is_completed[i] == false)
                {
                    if (bt_remaining[i] < minimum)
                    {
                        minimum = bt_remaining[i];
                        min_index = i;
                    }

                    if (bt_remaining[i] == minimum)
                    {
                        if (ps[i].at < ps[min_index].at)
                        {
                            minimum = bt_remaining[i];
                            min_index = i;
                        }
                    }
                }
            }

            if (min_index == -1)
            {
                current_time++;
            }
            else{
            if(bt_remaining[min_index] == ps[min_index].bt){
                ps[min_index].start_time = current_time;
                total_idle_time += (is_first == true) ? 0 : (ps[min_index].start_time - prev_process_completion_time);
                is_first = false;
            }

            bt_remaining[min_index] -= 1;
            current_time++;
            prev_process_completion_time = current_time;

            if(bt_remaining[min_index] == 0){

                ps[min_index].ct = current_time;
                ps[min_index].tat = ps[min_index].ct - ps[min_index].at;
                ps[min_index].wt = ps[min_index].tat - ps[min_index].bt;
                ps[min_index].rt = ps[min_index].start_time - ps[min_index].at;

                sum_tat +=ps[min_index].tat;
                sum_wt += ps[min_index].wt;
                sum_rt += ps[min_index].rt;

                completed++;
                is_completed[min_index] = true;
            }
        }
    }

    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;

    for (int i = 0; i < n; i++){
        max_completion_time = max(max_completion_time, ps[i].ct);
        min_arrival_time = min(min_arrival_time, ps[i].at);
    }

    total_cycle_length = max_completion_time - min_arrival_time;

    //Output
    cout<<"\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
    for(int i=0;i<n;i++)
     cout<<i<<"\t\t"<<ps[i].at<<"\t"<<ps[i].bt<<"\t\t"<<ps[i].ct<<"\t"<<ps[i].tat<<"\t"<<ps[i].wt<<"\t"<<ps[i].rt<<endl;
    cout<<endl;  
    
    cpu_utilization = float(total_cycle_length - total_idle_time)/ total_cycle_length;

    cout<<"\nAverage Turn Around time= "<< (float)sum_tat/n;
    cout<<"\nAverage Waiting Time= "<<(float)sum_wt/n;
    cout<<"\nAverage Response Time= "<<(float)sum_rt/n;    
    cout<<"\nThroughput= "<<n/total_cycle_length;
    cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;
    cout << endl;
    cout << endl;
}

void priority_non_preemptive(int n){

    cout << "Enter The Priority of Processes" << endl;
    cout << endl;
    for (int i = 0; i < n; i++){
        cout << "Priority of Process " << i << " is: ";
        cin >> ps[i].priority;
        cout << endl;
    }
    
    float sum_tat = 0.0, sum_wt = 0.0, sum_rt = 0.0;
    float total_cycle_length = 0;
    float total_idle_time = 0;
    float cpu_utilization = 0.0;
    bool is_first = true;
    int current_time = 0;
    int completed = 0;
    int prev_process_completion_time = 0;

    int max_completion_time = 0;
    int min_arrival_time = 0;

    vector<bool> is_completed(n, false);

    cout << fixed << setprecision(2);


    while (completed != n) {
        int max_index = -1;
        int maximum = -1;

        for (int i = 0; i < n; i++) {
            if (ps[i].at <= current_time && is_completed[i] == false) {
                if (ps[i].priority > maximum) {
                    maximum = ps[i].priority;
                    max_index = i;
                }
                if (ps[i].priority == maximum) {
                    if (ps[i].at < ps[max_index].at) {
                        maximum = ps[i].priority;
                        max_index = i;
                    }
                }
            }
        }

        if (max_index == -1) {
            current_time++;
        } else {
            ps[max_index].start_time = current_time;
            ps[max_index].ct = ps[max_index].start_time + ps[max_index].bt;
            ps[max_index].tat = ps[max_index].ct - ps[max_index].at;
            ps[max_index].wt = ps[max_index].tat - ps[max_index].bt;
            ps[max_index].rt = ps[max_index].start_time - ps[max_index].at;
            
            total_idle_time += (is_first == true) ? 0 : (ps[max_index].start_time - prev_process_completion_time);
            
            sum_tat += ps[max_index].tat;
            sum_wt += ps[max_index].wt;
            sum_rt += ps[max_index].rt;
            completed++;
            is_completed[max_index] = true;
            current_time = ps[max_index].ct;
            prev_process_completion_time = current_time;
            is_first = false;
        }
    }

    // Calculate Length of Process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;

    for (int i = 0; i < n; i++) {
        max_completion_time = max(max_completion_time, ps[i].ct);
        min_arrival_time = min(min_arrival_time, ps[i].at);
    }
    
    total_cycle_length = max_completion_time - min_arrival_time;
    cpu_utilization = (float)(total_cycle_length - total_idle_time) / total_cycle_length;

    // Output
    cout << "\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++)
        cout << i << "\t\t" << ps[i].at << "\t" << ps[i].bt << "\t\t" << ps[i].ct << "\t" << ps[i].tat << "\t" << ps[i].wt << "\t" << ps[i].rt << endl;
    
    cout << endl;
    cout << "\nAverage Turn Around time = " << (float)sum_tat / n;
    cout << "\nAverage Waiting Time = " << (float)sum_wt / n;
    cout << "\nAverage Response Time = " << (float)sum_rt / n;
    cout << "\nThroughput = " << n / total_cycle_length;
    cout << "\nCPU Utilization (Percentage) = " << cpu_utilization * 100;
    cout << endl;
    cout << endl;
}

void priority_preemptive(int n){

    cout << "Enter The Priority of Processes" << endl;
    cout << endl;
    for (int i = 0; i < n; i++){
        cout << "Priority of Process " << i << " is: ";
        cin >> ps[i].priority;
        cout << endl;
    }
    float sum_tat = 0.0, sum_wt = 0.0, sum_rt = 0.0;
    float total_cycle_length = 0;
    float total_idle_time = 0;
    float cpu_utilization = 0.0;
    bool is_first = true;
    int current_time = 0;
    int completed = 0;
    int prev_process_completion_time = 0;

    int max_completion_time = 0;
    int min_arrival_time = 0;
    vector<bool> is_completed(n, false);

    cout << fixed << setprecision(2);

    while (completed != n) {
        int max_index = -1;
        int maximum = -1;

        for (int i = 0; i < n; i++) {
            if (ps[i].at <= current_time && is_completed[i] == false) {
                if (ps[i].priority > maximum) {
                    maximum = ps[i].priority;
                    max_index = i;
                }
                if (ps[i].priority == maximum) {
                    if (ps[i].at < ps[max_index].at) {
                        maximum = ps[i].priority;
                        max_index = i;
                    }
                }
            }
        }

        if (max_index == -1) {
            current_time++;  // No process is ready, increment time
        } else {
            if (ps[max_index].remaining_time == ps[max_index].bt) {
                ps[max_index].start_time = current_time;  // First time CPU starts executing the process
            }

            ps[max_index].remaining_time--;  // Process executes for 1 unit of time
            current_time++;  // Time progresses

            if (ps[max_index].remaining_time == 0) {  // Process completed
                ps[max_index].ct = current_time;
                ps[max_index].tat = ps[max_index].ct - ps[max_index].at;
                ps[max_index].wt = ps[max_index].tat - ps[max_index].bt;
                ps[max_index].rt = ps[max_index].start_time - ps[max_index].at;

                sum_tat += ps[max_index].tat;
                sum_wt += ps[max_index].wt;
                sum_rt += ps[max_index].rt;
                completed++;
                is_completed[max_index] = true;

                total_idle_time += (is_first == true) ? 0 : (ps[max_index].start_time - prev_process_completion_time);
                prev_process_completion_time = current_time;
                is_first = false;
            }
        }
    }

    // Calculate Length of Process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;

    for (int i = 0; i < n; i++) {
        max_completion_time = max(max_completion_time, ps[i].ct);
        min_arrival_time = min(min_arrival_time, ps[i].at);
    }

    total_cycle_length = max_completion_time - min_arrival_time;
    cpu_utilization = (float)(total_cycle_length - total_idle_time) / total_cycle_length;

    // Output with Start Time
    cout << "\nProcess No.\tAT\tBT\tPriority\tST\tCT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++) {
        cout << i << "\t\t" << ps[i].at << "\t" << ps[i].bt << "\t" << ps[i].priority << "\t\t" 
             << ps[i].start_time << "\t" << ps[i].ct << "\t" << ps[i].tat << "\t" << ps[i].wt << "\t" << ps[i].rt << endl;
    }

    cout << endl;
    cout << "\nAverage Turn Around time = " << (float)sum_tat / n;
    cout << "\nAverage Waiting Time = " << (float)sum_wt / n;
    cout << "\nAverage Response Time = " << (float)sum_rt / n;
    cout << "\nThroughput = " << n / total_cycle_length;
    cout << "\nCPU Utilization (Percentage) = " << cpu_utilization * 100;
    cout << endl;
    cout << endl;

}




void round_robin(int n){

    float sum_tat = 0.0, sum_wt = 0.0, sum_rt = 0.0;
    float total_cycle_length = 0.0;
    float total_idle_time = 0.0;
    float cpu_utilization = 0.0;

    queue<int> qu;
    vector<bool> visited(n, false);
    bool is_first = true;
    int current_time = 0;
    int completed = 0;
    int index;
    int tq;

    int max_completion_time = 0;
    int min_arrival_time = 0;

    cout << fixed << setprecision(2) << endl;

    cout << endl;
    cout << endl;
    cout << "Enter the Time Sharing Quantum of CPU: ";
    cin >> tq;
    cout << endl;

    sort(ps.begin(), ps.end(), lambda);

    qu.push(0);
    visited[0] = true;

    while(completed != n){
        index = qu.front();
        qu.pop();
        if(ps[index].bt_remaining == ps[index].bt){
            ps[index].start_time = max(current_time, ps[index].at);
            total_idle_time += (is_first == true) ? 0 : (ps[index].start_time - current_time);
            current_time = ps[index].start_time;
            is_first = false;
        }

        if(ps[index].bt_remaining > tq){
            ps[index].bt_remaining -= tq;
            current_time += tq;
        }
        else{
            current_time += ps[index].bt_remaining;
            ps[index].bt_remaining = 0;
            completed++;

            ps[index].ct = current_time;
            ps[index].tat = ps[index].ct - ps[index].at;
            ps[index].wt = ps[index].tat - ps[index].bt;
            ps[index].rt = ps[index].start_time - ps[index].at;

            sum_tat += ps[index].tat;
            sum_wt += ps[index].wt;
            sum_rt += ps[index].rt;
            
        }

         //check which new Processes needs to be pushed to Ready Queue from Input list

        for (int i = 1; i < n; i++){
            if(ps[i].bt_remaining > 0 && ps[i].at <= current_time && visited[i] == false){
                qu.push(i);
                visited[i] = true;
            }
        }

        //check if Process on CPU needs to be pushed to Ready Queue

        if(ps[index].bt_remaining > 0){
            qu.push(index);
        }

        //if queue is empty, just add one process from list, whose remaining burst time > 0

        if(qu.empty()){
            for (int i = 0; i < n; i++){
                if(ps[i].bt_remaining > 0){
                    qu.push(i);
                    visited[i] = true;
                    break;
                }
            }
        }
    }

    //Calculate Length of Process completion cycle


    max_completion_time = INT_MIN;
    for (int i = 0; i < n; i++){
        max_completion_time = max(max_completion_time, ps[i].ct);
    }

    total_cycle_length = max_completion_time - ps[0].at;

    cpu_utilization = (total_cycle_length - total_idle_time)/ total_cycle_length;

    //sort so that process ID in output comes in Original order (just for interactivity- Not needed otherwise)

    sort(ps.begin(), ps.end(), cmp);

    //Output
  cout<<"\nProcess No.\tAT\tCPU Burst Time\tStart Time\tCT\tTAT\tWT\tRT\n";
  for(int i=0;i<n;i++)
    cout<<i<<"\t\t"<<ps[i].at<<"\t"<<ps[i].bt<<"\t\t"<<ps[i].start_time<<"\t\t"<<ps[i].ct<<"\t"<<ps[i].tat<<"\t"<<ps[i].wt<<"\t"<<ps[i].rt<<endl;
  cout<<endl;    

  cout<<"\nAverage Turn Around time= "<< (float)sum_tat/n;
  cout<<"\nAverage Waiting Time= "<<(float)sum_wt/n;
  cout<<"\nAverage Response Time= "<<(float)sum_rt/n;    
  cout<<"\nThroughput= "<<n/(float)total_cycle_length;
  cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;

  cout << endl;
  cout << endl;
}

int main()
{
    cout << endl;
    cout << "Enter The Total Number of Process: ";
    int n;
    cin >> n;
    ps.resize(n);
    cout << endl;


    cout << "Enter The Arrival Time of Processes" << endl;
    cout << endl;
    for (int i = 0; i < n; i++){
        cout << "Arrival Time of Process " << i << " is: ";
        cin >> ps[i].at;
        ps[i].pid = i;
        cout << endl;
    }
    cout << "Enter The Burst Time of Processes" << endl;
    cout << endl;
    for (int i = 0; i < n; i++){
        cout << "Burst Time of Process " << i << " is: ";
        cin >> ps[i].bt;
        ps[i].remaining_time = ps[i].bt;
        ps[i].bt_remaining = ps[i].bt;
        cout << endl;
    }
    fcfs(n);
    sjf_non_preemptive(n);
    sjf_preemptive(n);
    priority_non_preemptive(n);

    priority_preemptive(n);

    round_robin(n);
    return 0;
}