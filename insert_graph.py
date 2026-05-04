import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

df_insert=pd.read_csv('insert.csv')
df_insert=df_insert[df_insert['order']<=5000]
df_insert_early=df_insert[df_insert['order']<=100]
df_insert_late=df_insert[df_insert['order']>100]
df_insert_split=df_insert[df_insert['order']<=1000]
df_point=pd.read_csv('point_search.csv')
df_point=df_point[df_point['order']<=5000]
df_point_early=df_point[df_point['order']<=100]
df_point_late=df_point[df_point['order']>100]
df_range=pd.read_csv('range.csv')
df_range=df_range[df_range['order']<=5000]
df_range_early=df_range[df_range['order']<=100]
df_range_late=df_range[df_range['order']>100]
df_delete=pd.read_csv('deletion.csv')
df_delete=df_delete[df_delete['order']<=5000]
df_delete_early=df_delete[df_delete['order']<=100]
df_delete_late=df_delete[df_delete['order']>100]

plt.style.use('default')
colors={'B':'#1f77b4','BPlus':'#ff7f0e','BStar':'#2ca02c'}
labels={'B':'B-Tree','BPlus':'B+ Tree','BStar':'B* Tree'}
window_size=50

with PdfPages('tree_performance_final.pdf') as pdf:
    fig,axes=plt.subplots(6,1,figsize=(10,30),dpi=300)
    fig.subplots_adjust(hspace=0.45)

    ax=axes[0]
    ax.plot(df_insert_early['order'],df_insert_early['B_time'],color=colors['B'],label=labels['B'],linewidth=2.0)
    ax.plot(df_insert_early['order'],df_insert_early['BPlus_time'],color=colors['BPlus'],label=labels['BPlus'],linewidth=2.0)
    ax.plot(df_insert_early['order'],df_insert_early['BStar_time'],color=colors['BStar'],label=labels['BStar'],linewidth=2.0)
    ax.set_title('Insertion Time (d = 3 to 100)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Time (ms)',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[1]
    ax.plot(df_insert_late['order'],df_insert_late['B_time'],color=colors['B'],label=labels['B'],linewidth=1.5,alpha=0.9)
    ax.plot(df_insert_late['order'],df_insert_late['BPlus_time'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.5,alpha=0.9)
    ax.plot(df_insert_late['order'],df_insert_late['BStar_time'],color=colors['BStar'],label=labels['BStar'],linewidth=1.5,alpha=0.9)
    ax.set_title('Insertion Time (d = 100 to 5000)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Time (ms)',fontsize=12)
    ax.set_ylim(bottom=df_insert_late[['B_time','BPlus_time','BStar_time']].min().min()*0.9)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[2]
    ax.plot(df_insert_split['order'],df_insert_split['B_split'],color=colors['B'],label=labels['B'],linewidth=1.5)
    ax.plot(df_insert_split['order'],df_insert_split['BPlus_split'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.5)
    ax.plot(df_insert_split['order'],df_insert_split['BStar_split'],color=colors['BStar'],label=labels['BStar'],linewidth=1.5)
    ax.set_title('Total Split Counts(Log Scale)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Number of Splits',fontsize=12)
    ax.set_yscale('log')
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[3]
    ax.plot(df_insert['order'],df_insert['B_util'],color=colors['B'],label=labels['B'],linewidth=1.0,alpha=0.8)
    ax.plot(df_insert['order'],df_insert['BPlus_util'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.0,alpha=0.8)
    ax.plot(df_insert['order'],df_insert['BStar_util'],color=colors['BStar'],label=labels['BStar'],linewidth=1.0,alpha=0.8)
    ax.set_title('Node Utilization',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Utilization Ratio',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[4]
    ax.plot(df_point_early['order'],df_point_early['B_time'],color=colors['B'],label=labels['B'],linewidth=1.0,alpha=0.8)
    ax.plot(df_point_early['order'],df_point_early['BPlus_time'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.0,alpha=0.8)
    ax.plot(df_point_early['order'],df_point_early['BStar_time'],color=colors['BStar'],label=labels['BStar'],linewidth=1.0,alpha=0.8)
    ax.set_title('Point Search Execution Time (d = 3 to 100)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Average Execution Time(ns)',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[5]
    ax.plot(df_point_late['order'],df_point_late['B_time'],color=colors['B'],linewidth=1.0,alpha=0.15)
    ax.plot(df_point_late['order'],df_point_late['BPlus_time'],color=colors['BPlus'],linewidth=1.0,alpha=0.15)
    ax.plot(df_point_late['order'],df_point_late['BStar_time'],color=colors['BStar'],linewidth=1.0,alpha=0.15)
    ax.plot(df_point_late['order'],df_point_late['B_time'].rolling(window=window_size).mean(),color=colors['B'],label=labels['B'],linewidth=2.0,alpha=1.0)
    ax.plot(df_point_late['order'],df_point_late['BPlus_time'].rolling(window=window_size).mean(),color=colors['BPlus'],label=labels['BPlus'],linewidth=2.0,alpha=1.0)
    ax.plot(df_point_late['order'],df_point_late['BStar_time'].rolling(window=window_size).mean(),color=colors['BStar'],label=labels['BStar'],linewidth=2.0,alpha=1.0)
    ax.set_title('Point Search Execution Time (d = 100 to 5000)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Average Execution Time(ns)',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    pdf.savefig(fig,bbox_inches='tight')
    plt.close()

    fig,axes=plt.subplots(4,1,figsize=(10,20),dpi=300)
    fig.subplots_adjust(hspace=0.45)

    ax=axes[0]
    ax.plot(df_range_early['order'],df_range_early['B_time'],color=colors['B'],label=labels['B'],linewidth=1.0,alpha=0.8)
    ax.plot(df_range_early['order'],df_range_early['BPlus_time'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.0,alpha=0.8)
    ax.plot(df_range_early['order'],df_range_early['BStar_time'],color=colors['BStar'],label=labels['BStar'],linewidth=1.0,alpha=0.8)
    ax.set_title('Range Search Execution Time (d = 3 to 100)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Average Execution Time(ns)',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[1]
    ax.plot(df_range_late['order'],df_range_late['B_time'],color=colors['B'],label=labels['B'],linewidth=1.0,alpha=0.8)
    ax.plot(df_range_late['order'],df_range_late['BPlus_time'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.0,alpha=0.8)
    ax.plot(df_range_late['order'],df_range_late['BStar_time'],color=colors['BStar'],label=labels['BStar'],linewidth=1.0,alpha=0.8)
    ax.set_title('Range Search Execution Time (d = 100 to 5000)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Average Execution Time(ns)',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[2]
    ax.plot(df_delete_early['order'],df_delete_early['B_time'],color=colors['B'],label=labels['B'],linewidth=1.0,alpha=0.8)
    ax.plot(df_delete_early['order'],df_delete_early['BPlus_time'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.0,alpha=0.8)
    ax.plot(df_delete_early['order'],df_delete_early['BStar_time'],color=colors['BStar'],label=labels['BStar'],linewidth=1.0,alpha=0.8)
    ax.set_title('Deletion Execution Time (d = 3 to 100)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Average Execution Time(ns)',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[3]
    ax.plot(df_delete_late['order'],df_delete_late['B_time'],color=colors['B'],linewidth=1.0,alpha=0.15)
    ax.plot(df_delete_late['order'],df_delete_late['BPlus_time'],color=colors['BPlus'],linewidth=1.0,alpha=0.15)
    ax.plot(df_delete_late['order'],df_delete_late['BStar_time'],color=colors['BStar'],linewidth=1.0,alpha=0.15)
    ax.plot(df_delete_late['order'],df_delete_late['B_time'].rolling(window=window_size).mean(),color=colors['B'],label=labels['B'],linewidth=2.0,alpha=1.0)
    ax.plot(df_delete_late['order'],df_delete_late['BPlus_time'].rolling(window=window_size).mean(),color=colors['BPlus'],label=labels['BPlus'],linewidth=2.0,alpha=1.0)
    ax.plot(df_delete_late['order'],df_delete_late['BStar_time'].rolling(window=window_size).mean(),color=colors['BStar'],label=labels['BStar'],linewidth=2.0,alpha=1.0)
    ax.set_title('Deletion Execution Time (d = 100 to 5000)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Average Execution Time(ns)',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    pdf.savefig(fig,bbox_inches='tight')
    plt.close()

    fig,axes=plt.subplots(2,1,figsize=(10,10),dpi=300)
    fig.subplots_adjust(hspace=0.45)

    ax=axes[0]
    ax.plot(df_delete_early['order'],df_delete_early['B_merge'],color=colors['B'],label=labels['B'],linewidth=1.0,alpha=0.8)
    ax.plot(df_delete_early['order'],df_delete_early['BPlus_merge'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.0,alpha=0.8)
    ax.plot(df_delete_early['order'],df_delete_early['BStar_merge'],color=colors['BStar'],label=labels['BStar'],linewidth=1.0,alpha=0.8)
    ax.set_title('Merge Count (d = 3 to 100)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Total Merge',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    ax=axes[1]
    ax.plot(df_delete_late['order'],df_delete_late['B_merge'],color=colors['B'],label=labels['B'],linewidth=1.0,alpha=0.8)
    ax.plot(df_delete_late['order'],df_delete_late['BPlus_merge'],color=colors['BPlus'],label=labels['BPlus'],linewidth=1.0,alpha=0.8)
    ax.plot(df_delete_late['order'],df_delete_late['BStar_merge'],color=colors['BStar'],label=labels['BStar'],linewidth=1.0,alpha=0.8)
    ax.set_title('Merge Count (d = 100 to 5000)',fontsize=14,fontweight='bold')
    ax.set_xlabel('Order (d)',fontsize=12)
    ax.set_ylabel('Total Merge',fontsize=12)
    ax.grid(True,linestyle='--',alpha=0.5)
    ax.legend(fontsize=11)

    pdf.savefig(fig,bbox_inches='tight')
    plt.close()

print(f"complete")