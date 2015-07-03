import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

df = pd.DataFrame.from_csv('test/output.csv', parse_dates=False)

select_list = ['demand_type','param1','param2']
for key, grp in df.groupby(select_list):
    plt.errorbar(grp['N'], grp['SAA_eps_avg'], yerr=grp['SAA_eps_conf'], label=key)

plt.title(select_list)
plt.xlabel('N')
#plt.xscale('log')
plt.ylabel('Epsilon')
plt.legend(loc='best')    

#plt.show()
plt.savefig('test/epsilon.png',format='png')

