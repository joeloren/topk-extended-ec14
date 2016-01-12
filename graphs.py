import matplotlib.pyplot as plt
import itertools
import os

def read_data_topk(rule, n, m):
    "read data from results directory"
    file_name = "results/%s:%d:%d" % (rule, n, m)
    data = file(file_name, "r").read().splitlines()[1:]
    return [range(1,m)] + [map(float, line.split()[1:]) for line in data]

def read_data_topk_mallows(rule, n, m):
    "read data from results directory"
    file_name = "results/%s:%d:%d:mallows" % (rule, n, m)
    data = file(file_name, "r").read().splitlines()[1:]
    transposed = [map(float, line.split()) for line in data]
    return [map(lambda x: x[i], transposed) for i in range(len(transposed[0]))]

def plot_data_topk(rule, n, m, title = None, save = False, show = False):
    "plot data from results directory"
    if title is None:
        title = "%s on %d candidates and %d voters" % (rule.capitalize(), m, n)
    data = read_data_topk(rule, n, m)
    if rule == 'copeland':
        alg_name = 'FairPWCutoff'
        legend_loc = 2
    else:
        alg_name = 'FairCutoff'
        legend_loc = 4
    plt.figure()
    if len(data) == 3:
        plt.plot(data[0], data[1], 'o-', data[0], data[2], '^--')
        plt.legend(['Naive', alg_name], loc = legend_loc)
    else:
        plt.plot(data[0], data[1], 'o-', data[0], data[2], '^--', data[0], data[3], 's:')
        plt.legend(['Naive', alg_name, 'Optimal'], loc = legend_loc)
    plt.ylim(0,1)
    plt.xlabel(r'$k$')
    plt.ylabel('Success probability')
    plt.title(title)
    if show:
        plt.show()
    if save:
        plt.savefig('figures/%s-%d-%d.pdf' % (rule, n, m), format='pdf', bbox_inches='tight')

def plot_data_topk_mallows(rule, n, m, title = None, save = False, show = False):
    "plot data from results directory"
    if title is None:
        title = "%s on %d candidates and %d voters" % (rule.capitalize(), m, n)
    data = read_data_topk_mallows(rule, n, m)
    plt.figure()
    plt.plot(data[0], data[1], 'o-', data[0], data[2], '^--')
    plt.legend(['Winner', 'First'], loc = 3)
    plt.ylim(0,1)
    plt.xlabel(r'$\phi$')
    plt.ylabel('Success probability')
    plt.title(title)
    if show:
        plt.show()
    if save:
        plt.savefig('figures/%s-%d-%d-mallows.pdf' % (rule, n, m), format='pdf', bbox_inches='tight')

def plot_data_topk_comparative(rules, n, m, title = None, save = False, show = False):
    "plot comparative data for Alg 1"
    #if title is None:
    #    title = "Algorithm 1 on %d candidates and %d voters" % (m, n)
    data = [read_data_topk(rule, n, m) for rule in rules]
    plt.figure()
    to_plot = []
    for (d, style) in zip(data, itertools.cycle(['o-', '^--', 's:', '+-.', 'x-'])):
        to_plot += [d[0], d[2], style]
    apply(plt.plot, to_plot)
    plt.legend([rule.capitalize() for rule in rules], loc = 4)
    plt.ylim(0,1)
    plt.xlabel(r'$k$')
    plt.ylabel('Success probability')
    if title is not None:
        plt.title(title)
    if show:
        plt.show()
    if save:
        plt.savefig('figures/comparative-%d-%d.pdf' % (n, m), format='pdf', bbox_inches='tight')

def plot_data_topk_mallows_comparative(rules, n, m, title = None, save = False, show = False):
    "plot comparative data for Mallows"
    #if title is None: # was: Zero elicitation
    #    title = "Leader recovery on %d candidates and %d voters" % (m, n)
    data = [read_data_topk_mallows(rule, n, m) for rule in rules]
    plt.figure()
    to_plot = []
    styles = itertools.cycle(['o-', '^--', 's:', '+-.', 'x-'])
    for d in data:
        to_plot += [d[0], d[1], styles.next()]
    to_plot += [data[0][0], data[0][2], styles.next()]
    apply(plt.plot, to_plot)
    plt.legend([rule.capitalize() for rule in rules] + ['First'], loc = 3)
    plt.ylim(0,1)
    plt.xlabel(r'$\phi$')
    plt.ylabel('Success probability')
    if title is not None:
        plt.title(title)
    if show:
        plt.show()
    if save:
        plt.savefig('figures/comparative-%d-%d-mallows.pdf' % (n, m), format='pdf', bbox_inches='tight')

def auto_plot(save = False, show = None):
    """
    Automatically plot everything in results directory.
    If save is True, then figures are saved to PDFs instead of being shown on screen.
    """
    if show is None:
        show = not save
    params_regular = dict([])
    params_mallows = dict([])
    for f in os.listdir('results'):
        if os.stat('results/' + f).st_size == 0: # skip empty files
            continue
        x = f.split(":")
        params = (int(x[1]), int(x[2]))
        if len(x) == 3:
            plot_data_topk(x[0], int(x[1]), int(x[2]), save = save)
            p = params_regular
        else:
            plot_data_topk_mallows(x[0], int(x[1]), int(x[2]), save = save)
            p = params_mallows
        if params not in p:
            p[params] = []
        p[params].append(x[0])
    for (n,m) in params_regular:
        plot_data_topk_comparative(params_regular[(n,m)], n, m, save = save)
    for (n,m) in params_mallows:
        plot_data_topk_mallows_comparative(params_mallows[(n,m)], n, m, save = save)
    if show:
        plt.show()
