#TEM QUE SETAR MANUALMENTE O NOME DO ARQUIVO CSV NA LINHA 16 PRA NÃO DAR ERRO

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
from pathlib import Path

# Configuração do estilo do matplotlib
plt.style.use('ggplot')

# Caminhos dos arquivos
current_dir = Path.cwd()
# Subir um nível para a pasta raiz do projeto
project_root = current_dir.parent
csv_path = project_root / "results/files/shellSort" / "shellsort_geral_2026-01-08_19-49-31.csv" # ATENÇÃO COM O NOME DO ARQUIVO
output_dir = project_root / "results/graphics" / "shellSort"
output_dir.mkdir(parents=True, exist_ok=True)

# Carregar dados
print(f"Carregando arquivo: {csv_path}")
df = pd.read_csv(csv_path, sep=';', na_filter=False)

print(f"\n📊 Informações do DataFrame:")
print(f"Total de linhas: {len(df)}")
print(f"Colunas: {list(df.columns)}")

# Identificar linhas de estatísticas (são as últimas linhas com valores diferentes)
# Vamos verificar quais linhas não têm números inteiros na coluna 'repeticao'
df['is_stat'] = False

# As linhas estatísticas têm valores diferentes na coluna 'repeticao'
# Vamos verificar o tipo dos dados
for idx, row in df.iterrows():
    try:
        # Tentar converter repeticao para inteiro
        int(row['repeticao'])
        df.at[idx, 'is_stat'] = False
    except (ValueError, TypeError):
        # Se não conseguir converter, é uma linha estatística
        df.at[idx, 'is_stat'] = True

# Separar dados brutos e estatísticas
df_raw = df[~df['is_stat']].copy()
df_stats = df[df['is_stat']].copy()

print(f"\n📈 Dados brutos: {len(df_raw)} linhas")
print(f"📊 Dados estatísticos: {len(df_stats)} linhas")

# Mostrar as estatísticas
if len(df_stats) > 0:
    print(f"\n📋 Dados estatísticos encontrados:")
    print(df_stats[['algoritmo', 'cenario', 'tamanho', 'repeticao', 'tempo_ms', 'comparacoes', 'trocas']])
    
    # Renomear as colunas das estatísticas
    # As colunas nas estatísticas são: repeticao=media_tempo_ms, tempo_ms=desvio_tempo_ms
    df_stats_clean = df_stats.copy()
    
    # Renomear colunas conforme a estrutura vista
    df_stats_clean = df_stats_clean.rename(columns={
        'repeticao': 'media_tempo_ms',
        'tempo_ms': 'desvio_tempo_ms',
        'comparacoes': 'media_comparacoes',
        'trocas': 'media_trocas'
    })
    
    # Remover coluna auxiliar
    df_stats_clean = df_stats_clean.drop(columns=['is_stat'])
    
    print(f"\n✅ Estatísticas após renomeação:")
    print(df_stats_clean[['algoritmo', 'cenario', 'tamanho', 'media_tempo_ms', 'desvio_tempo_ms', 
                         'media_comparacoes', 'media_trocas']])
    
    # Converter para tipos numéricos
    df_stats_clean['tamanho'] = pd.to_numeric(df_stats_clean['tamanho'], errors='coerce')
    df_stats_clean['media_tempo_ms'] = pd.to_numeric(df_stats_clean['media_tempo_ms'], errors='coerce')
    df_stats_clean['desvio_tempo_ms'] = pd.to_numeric(df_stats_clean['desvio_tempo_ms'], errors='coerce')
    df_stats_clean['media_comparacoes'] = pd.to_numeric(df_stats_clean['media_comparacoes'], errors='coerce')
    df_stats_clean['media_trocas'] = pd.to_numeric(df_stats_clean['media_trocas'], errors='coerce')
    
    # Limpar dados
    df_stats_clean = df_stats_clean.dropna(subset=['cenario', 'tamanho'])
    df_stats_clean['cenario'] = df_stats_clean['cenario'].astype(str).str.strip()
    df_raw['cenario'] = df_raw['cenario'].astype(str).str.strip()
    
    df_stats = df_stats_clean
else:
    # Calcular estatísticas dos dados brutos
    print("\n⚠️  Calculando estatísticas a partir dos dados brutos...")
    df_stats = df_raw.groupby(['algoritmo', 'cenario', 'tamanho']).agg({
        'tempo_ms': ['mean', 'std'],
        'comparacoes': 'mean',
        'trocas': 'mean'
    }).reset_index()
    
    # Renomear colunas
    df_stats.columns = ['algoritmo', 'cenario', 'tamanho', 'media_tempo_ms', 'desvio_tempo_ms', 
                       'media_comparacoes', 'media_trocas']
    
    # Converter para tipos numéricos
    df_stats['tamanho'] = pd.to_numeric(df_stats['tamanho'], errors='coerce')
    df_stats['media_tempo_ms'] = pd.to_numeric(df_stats['media_tempo_ms'], errors='coerce')
    df_stats['desvio_tempo_ms'] = pd.to_numeric(df_stats['desvio_tempo_ms'], errors='coerce')
    df_stats['media_comparacoes'] = pd.to_numeric(df_stats['media_comparacoes'], errors='coerce')
    df_stats['media_trocas'] = pd.to_numeric(df_stats['media_trocas'], errors='coerce')
    
    # Descartar dados inválidos
    df_stats = df_stats.dropna(subset=['cenario', 'tamanho'])
    df_stats['cenario'] = df_stats['cenario'].astype(str).str.strip()

print(f"\n✅ Dados processados com sucesso!")
print(f"📊 Estatísticas disponíveis: {len(df_stats)} linhas")
if len(df_stats) > 0:
    print(f"\nResumo das estatísticas:")
    print(df_stats[['algoritmo', 'cenario', 'tamanho', 'media_tempo_ms', 'desvio_tempo_ms', 
                   'media_comparacoes', 'media_trocas']].to_string(index=False))

# ============================================
# 1. GRÁFICOS POR CENÁRIO INDIVIDUAL
# ============================================

# Função para criar gráfico individual por cenário
def plot_individual_scenario(cenario, df_data, df_stats_data):
    # Converter cenario para string
    cenario_str = str(cenario)
    
    fig, axes = plt.subplots(2, 3, figsize=(18, 10))
    fig.suptitle(f'Shell Sort - Cenário: {cenario_str.capitalize()}', fontsize=16, fontweight='bold')
    
    # Filtrar dados
    stats_cenario = df_stats_data[df_stats_data['cenario'] == cenario_str].copy()
    
    # Ordenar por tamanho
    stats_cenario = stats_cenario.sort_values('tamanho')
    
    # Verificar se temos dados suficientes
    if len(stats_cenario) == 0:
        print(f"⚠️  Nenhum dado estatístico para o cenário: {cenario_str}")
        plt.close()
        return
    
    print(f"\n📈 Plotando gráfico para cenário: {cenario_str}")
    print(f"   Tamanhos disponíveis: {list(stats_cenario['tamanho'])}")
    
    # Gráfico 1: Tempo médio vs Tamanho
    ax1 = axes[0, 0]
    ax1.plot(stats_cenario['tamanho'], stats_cenario['media_tempo_ms'], 
             marker='o', linewidth=2, markersize=8, color='steelblue')
    ax1.set_xlabel('Tamanho do Array')
    ax1.set_ylabel('Tempo Médio (ms)')
    ax1.set_title(f'Tempo Médio de Execução\nCenário: {cenario_str}')
    ax1.grid(True, alpha=0.3)
    
    # Adicionar valores nos pontos
    for x, y in zip(stats_cenario['tamanho'], stats_cenario['media_tempo_ms']):
        ax1.text(x, y, f'{y:.2f}', ha='center', va='bottom', fontsize=9)
    
    # Gráfico 2: Desvio padrão do tempo vs Tamanho
    ax2 = axes[0, 1]
    ax2.plot(stats_cenario['tamanho'], stats_cenario['desvio_tempo_ms'], 
             marker='s', linewidth=2, markersize=8, color='darkorange')
    ax2.set_xlabel('Tamanho do Array')
    ax2.set_ylabel('Desvio Padrão (ms)')
    ax2.set_title(f'Variação do Tempo de Execução\nCenário: {cenario_str}')
    ax2.grid(True, alpha=0.3)
    
    # Gráfico 3: Comparações vs Tamanho
    ax3 = axes[0, 2]
    ax3.plot(stats_cenario['tamanho'], stats_cenario['media_comparacoes'], 
             marker='^', linewidth=2, markersize=8, color='forestgreen')
    ax3.set_xlabel('Tamanho do Array')
    ax3.set_ylabel('Número de Comparações')
    ax3.set_title(f'Comparações Realizadas\nCenário: {cenario_str}')
    ax3.grid(True, alpha=0.3)
    
    # Gráfico 4: Trocas vs Tamanho (se houver trocas)
    ax4 = axes[1, 0]
    ax4.plot(stats_cenario['tamanho'], stats_cenario['media_trocas'], 
             marker='d', linewidth=2, markersize=8, color='firebrick')
    ax4.set_xlabel('Tamanho do Array')
    ax4.set_ylabel('Número de Trocas')
    ax4.set_title(f'Trocas Realizadas\nCenário: {cenario_str}')
    ax4.grid(True, alpha=0.3)
    
    # Gráfico 5: Razão Comparações/Tamanho
    ax5 = axes[1, 1]
    comparacoes_por_elemento = stats_cenario['media_comparacoes'] / stats_cenario['tamanho']
    ax5.plot(stats_cenario['tamanho'], comparacoes_por_elemento, 
             marker='v', linewidth=2, markersize=8, color='rebeccapurple')
    ax5.set_xlabel('Tamanho do Array')
    ax5.set_ylabel('Comparações por Elemento')
    ax5.set_title(f'Eficiência: Comparações por Elemento\nCenário: {cenario_str}')
    ax5.grid(True, alpha=0.3)
    
    # Gráfico 6: Coeficiente de Variação
    ax6 = axes[1, 2]
    coef_variacao = (stats_cenario['desvio_tempo_ms'] / stats_cenario['media_tempo_ms']) * 100
    ax6.plot(stats_cenario['tamanho'], coef_variacao, 
             marker='*', linewidth=2, markersize=10, color='saddlebrown')
    ax6.set_xlabel('Tamanho do Array')
    ax6.set_ylabel('Coeficiente de Variação (%)')
    ax6.set_title(f'Consistência do Tempo\nCenário: {cenario_str}')
    ax6.grid(True, alpha=0.3)
    
    plt.tight_layout()
    filename = f'shellsort_{cenario_str}_individual.png'
    plt.savefig(output_dir / filename, dpi=300, bbox_inches='tight')
    plt.close()
    print(f"   ✅ Gráfico salvo: {filename}")

# Criar gráficos para cada cenário
if len(df_stats) > 0:
    cenarios_unicos = df_stats['cenario'].unique()
    print(f"\n🎯 Cenários encontrados nas estatísticas: {len(cenarios_unicos)}")
    for i, cenario in enumerate(cenarios_unicos, 1):
        print(f"  {i}. '{cenario}'")
        plot_individual_scenario(cenario, df_raw, df_stats)
else:
    print("⚠️  Nenhum dado estatístico disponível para criar gráficos")

# ============================================
# 2. GRÁFICOS COM ESTATÍSTICAS SIMILARES MESCLADAS
# ============================================

if len(df_stats) > 0:
    # Paleta de cores para os cenários
    cores_cenarios = {
        'crescente': 'steelblue',
        'decrescente': 'darkorange',
        'aleatorio': 'forestgreen'
    }

    print(f"\n🎨 Criando gráficos comparativos...")
    
    # Gráfico 1: Comparação de Tempo Médio entre Cenários
    plt.figure(figsize=(12, 8))
    
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        plt.plot(subset['tamanho'], subset['media_tempo_ms'], 
                 marker='o', linewidth=2.5, markersize=9, 
                 color=cor,
                 label=label)

    plt.xlabel('Tamanho do Array', fontsize=12, fontweight='bold')
    plt.ylabel('Tempo Médio (ms)', fontsize=12, fontweight='bold')
    plt.title('Comparação de Tempo Médio de Execução\nShell Sort - Todos os Cenários', 
              fontsize=14, fontweight='bold')
    plt.legend(title='Cenário', title_fontsize=12, fontsize=11)
    plt.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_comparacao_tempo_cenarios.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 1 salvo: shellsort_comparacao_tempo_cenarios.png")
    
    # Gráfico 2: Comparação de Número de Comparações
    plt.figure(figsize=(12, 8))
    
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        plt.plot(subset['tamanho'], subset['media_comparacoes'], 
                 marker='s', linewidth=2.5, markersize=8, 
                 color=cor,
                 label=label)

    plt.xlabel('Tamanho do Array', fontsize=12, fontweight='bold')
    plt.ylabel('Número de Comparações', fontsize=12, fontweight='bold')
    plt.title('Comparação de Operações de Comparação\nShell Sort - Todos os Cenários', 
              fontsize=14, fontweight='bold')
    plt.legend(title='Cenário', title_fontsize=12, fontsize=11)
    plt.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_comparacao_comparacoes.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 2 salvo: shellsort_comparacao_comparacoes.png")
    
    # Gráfico 3: Comparação de Consistência (Coeficiente de Variação)
    plt.figure(figsize=(12, 8))
    
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        coef_variacao = (subset['desvio_tempo_ms'] / subset['media_tempo_ms']) * 100
        plt.plot(subset['tamanho'], coef_variacao, 
                 marker='^', linewidth=2.5, markersize=9, 
                 color=cor,
                 label=label)

    plt.xlabel('Tamanho do Array', fontsize=12, fontweight='bold')
    plt.ylabel('Coeficiente de Variação (%)', fontsize=12, fontweight='bold')
    plt.title('Consistência do Tempo de Execução\nMenor = Mais Consistente', 
              fontsize=14, fontweight='bold')
    plt.legend(title='Cenário', title_fontsize=12, fontsize=11)
    plt.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_consistencia_cenarios.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 3 salvo: shellsort_consistencia_cenarios.png")
    
    # Gráfico 4: Eficiência - Comparações por Elemento
    plt.figure(figsize=(12, 8))
    
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        comparacoes_por_elemento = subset['media_comparacoes'] / subset['tamanho']
        plt.plot(subset['tamanho'], comparacoes_por_elemento, 
                 marker='d', linewidth=2.5, markersize=8, 
                 color=cor,
                 label=label)

    plt.xlabel('Tamanho do Array', fontsize=12, fontweight='bold')
    plt.ylabel('Comparações por Elemento', fontsize=12, fontweight='bold')
    plt.title('Eficiência do Algoritmo\nComparações por Elemento (Menor = Mais Eficiente)', 
              fontsize=14, fontweight='bold')
    plt.legend(title='Cenário', title_fontsize=12, fontsize=11)
    plt.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_eficiencia_comparacoes.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 4 salvo: shellsort_eficiencia_comparacoes.png")
    
    # Gráfico 5: Eficiência - Trocas por Elemento
    plt.figure(figsize=(12, 8))
    
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        trocas_por_elemento = subset['media_trocas'] / subset['tamanho']
        plt.plot(subset['tamanho'], trocas_por_elemento, 
                 marker='v', linewidth=2.5, markersize=8, 
                 color=cor,
                 label=label)

    plt.xlabel('Tamanho do Array', fontsize=12, fontweight='bold')
    plt.ylabel('Trocas por Elemento', fontsize=12, fontweight='bold')
    plt.title('Eficiência em Trocas\nShell Sort - Todos os Cenários', 
              fontsize=14, fontweight='bold')
    plt.legend(title='Cenário', title_fontsize=12, fontsize=11)
    plt.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_trocas_por_elemento.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 5 salvo: shellsort_trocas_por_elemento.png")

    # Gráfico 6: Comparação de Trocas entre Cenários
    plt.figure(figsize=(12, 8))
    
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        plt.plot(subset['tamanho'], subset['media_trocas'], 
                 marker='D', linewidth=2.5, markersize=9, 
                 color=cor,
                 label=label)

    plt.xlabel('Tamanho do Array', fontsize=12, fontweight='bold')
    plt.ylabel('Número Médio de Trocas', fontsize=12, fontweight='bold')
    plt.title('Comparação de Trocas Realizadas\nShell Sort - Todos os Cenários', 
              fontsize=14, fontweight='bold')
    plt.legend(title='Cenário', title_fontsize=12, fontsize=11)
    plt.grid(True, alpha=0.3)

    # Adicionar anotações com valores
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        # Anotar o último ponto
        last_point = subset.iloc[-1]
        plt.annotate(f'{last_point["media_trocas"]:,.0f}', 
                     xy=(last_point['tamanho'], last_point['media_trocas']),
                     xytext=(0, 10), textcoords='offset points',
                     ha='center', fontsize=10,
                     bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.8))

    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_comparacao_trocas_cenarios.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 6 salvo: shellsort_comparacao_trocas_cenarios.png")
    
    # Gráfico 7: Análise de Complexidade (Escala Log-Log)
    plt.figure(figsize=(12, 8))
    
    # Converter para arrays numpy para cálculos
    tamanhos = np.array(sorted(df_stats['tamanho'].unique()))
    
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        tempos = np.array(subset['media_tempo_ms'])
        plt.loglog(subset['tamanho'], tempos, 
                   marker='o', linewidth=2, markersize=8, 
                   color=cor,
                   label=label)
        
        # Calcular coeficiente angular (aproximação da complexidade)
        if len(tempos) > 1:
            log_tamanhos = np.log10(subset['tamanho'])
            log_tempos = np.log10(tempos)
            coef = np.polyfit(log_tamanhos, log_tempos, 1)[0]
            
            # Anotar o coeficiente
            last_point = subset.iloc[-1]
            plt.annotate(f'α ≈ {coef:.2f}', 
                         xy=(last_point['tamanho'], last_point['media_tempo_ms']),
                         xytext=(10, 0), textcoords='offset points',
                         ha='left', fontsize=9,
                         bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.8))

    # Linhas de referência para complexidades
    plt.loglog(tamanhos, tamanhos * np.log10(tamanhos), 'k--', linewidth=1, alpha=0.5, label='O(n log n)')
    plt.loglog(tamanhos, tamanhos**1.5, 'k:', linewidth=1, alpha=0.5, label='O(n^1.5)')
    
    plt.xlabel('Tamanho do Array (escala log)', fontsize=12, fontweight='bold')
    plt.ylabel('Tempo Médio (ms) (escala log)', fontsize=12, fontweight='bold')
    plt.title('Análise de Complexidade do Shell Sort\nEscala Log-Log', 
              fontsize=14, fontweight='bold')
    plt.legend(title='Cenário / Complexidade', title_fontsize=12, fontsize=11)
    plt.grid(True, alpha=0.3, which='both')

    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_analise_complexidade.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 7 salvo: shellsort_analise_complexidade.png")
    
    # Gráfico 8: Resumo de Desempenho (4 em 1)
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle('Resumo de Desempenho - Shell Sort', fontsize=16, fontweight='bold')
    
    # Subplot 1: Tempo Médio
    ax1 = axes[0, 0]
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        ax1.plot(subset['tamanho'], subset['media_tempo_ms'], 
                 marker='o', linewidth=2, markersize=6,
                 color=cor,
                 label=label)
    ax1.set_xlabel('Tamanho do Array')
    ax1.set_ylabel('Tempo Médio (ms)')
    ax1.set_title('Tempo de Execução')
    ax1.legend(fontsize=9)
    ax1.grid(True, alpha=0.3)
    
    # Subplot 2: Comparações por Elemento
    ax2 = axes[0, 1]
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        comparacoes_por_elemento = subset['media_comparacoes'] / subset['tamanho']
        ax2.plot(subset['tamanho'], comparacoes_por_elemento, 
                 marker='s', linewidth=2, markersize=6,
                 color=cor,
                 label=label)
    ax2.set_xlabel('Tamanho do Array')
    ax2.set_ylabel('Comparações por Elemento')
    ax2.set_title('Eficiência em Comparações')
    ax2.legend(fontsize=9)
    ax2.grid(True, alpha=0.3)
    
    # Subplot 3: Trocas por Elemento
    ax3 = axes[1, 0]
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        trocas_por_elemento = subset['media_trocas'] / subset['tamanho']
        ax3.plot(subset['tamanho'], trocas_por_elemento, 
                 marker='^', linewidth=2, markersize=6,
                 color=cor,
                 label=label)
    ax3.set_xlabel('Tamanho do Array')
    ax3.set_ylabel('Trocas por Elemento')
    ax3.set_title('Eficiência em Trocas')
    ax3.legend(fontsize=9)
    ax3.grid(True, alpha=0.3)
    
    # Subplot 4: Coeficiente de Variação
    ax4 = axes[1, 1]
    for cenario in cenarios_unicos:
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str].copy()
        subset = subset.sort_values('tamanho')
        
        cor = cores_cenarios.get(cenario_str, 'gray')
        label = cenario_str.capitalize() if cenario_str in cores_cenarios else cenario_str
        
        coef_variacao = (subset['desvio_tempo_ms'] / subset['media_tempo_ms']) * 100
        ax4.plot(subset['tamanho'], coef_variacao, 
                 marker='d', linewidth=2, markersize=6,
                 color=cor,
                 label=label)
    ax4.set_xlabel('Tamanho do Array')
    ax4.set_ylabel('Coef. Variação (%)')
    ax4.set_title('Consistência')
    ax4.legend(fontsize=9)
    ax4.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig(output_dir / 'shellsort_resumo_geral.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("  ✅ Gráfico 8 salvo: shellsort_resumo_geral.png")
    
    print(f"\n✅ Todos os gráficos foram criados com sucesso!")
    print(f"📁 Pasta de saída: {output_dir}")
    
    # Mostrar estatísticas básicas
    print(f"\n📊 Estatísticas por Cenário:")
    print("-" * 60)
    
    for cenario in sorted(cenarios_unicos):
        cenario_str = str(cenario)
        subset = df_stats[df_stats['cenario'] == cenario_str]
        
        if not subset.empty:
            print(f"\nCenário: {cenario_str.upper()}")
            print(f"  Tamanhos analisados: {', '.join(map(str, sorted(subset['tamanho'].unique())))}")
            
            # Tempo para o maior tamanho
            max_tamanho = subset['tamanho'].max()
            tempo_max = subset[subset['tamanho'] == max_tamanho]['media_tempo_ms'].values[0]
            print(f"  Tempo para {max_tamanho:,} elementos: {tempo_max:.2f} ms")
            
            # Comparações por elemento
            comparacoes_por_elemento = subset[subset['tamanho'] == max_tamanho]['media_comparacoes'].values[0] / max_tamanho
            print(f"  Comparações por elemento: {comparacoes_por_elemento:.1f}")
            
            # Trocas por elemento
            trocas_por_elemento = subset[subset['tamanho'] == max_tamanho]['media_trocas'].values[0] / max_tamanho
            print(f"  Trocas por elemento: {trocas_por_elemento:.1f}")
    
    print(f"\n📋 Total de gráficos gerados: {len(list(output_dir.glob('*.png')))}")
    
else:
    print("⚠️  Nenhum dado estatístico disponível para criar gráficos comparativos")

print(f"\n🎉 Processamento concluído!")