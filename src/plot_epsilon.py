import matplotlib.pyplot as plt
import pandas as pd

df = pd.DataFrame.from_csv('test/output.csv', parse_dates=False)

plt.figure()
plt.clf()
plt.errorbar(df.N,df.SAA_eps_avg,xerr=0,yerr=df.SAA_eps_conf,marker='x',c='r',ls='-')
#plt.errorbar(df.N,df.SAA_eps_avg_interp,xerr=0,yerr=df.SAA_eps_conf_interp,marker='x',c='g',ls='-')

plt.axis([4, 16, 0.05, 0.25])
plt.xlabel('N')
#plt.xscale('log')
plt.ylabel('Epsilon')
plt.legend( ('SAA_eps_avg','SAA_eps_avg_interp'),\
             loc='upper right')

#plt.show()
plt.savefig('test/epsilon.png',format='png')

