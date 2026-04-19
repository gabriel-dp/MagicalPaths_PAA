#include "../include/dijkstra.h"

#include <limits.h>
#include <stdlib.h>

MinHeap* createHeap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->data = malloc(sizeof(MinHeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void freeHeap(MinHeap* heap) {
    free(heap->data);
    free(heap);
}

int isEmpty(MinHeap* heap) {
    return heap->size == 0;
}

void swap(MinHeapNode* a, MinHeapNode* b) {
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;

        if (heap->data[parent].dist <= heap->data[i].dist)
            break;

        swap(&heap->data[parent], &heap->data[i]);
        i = parent;
    }
}

void heapifyDown(MinHeap* heap, int i) {
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heap->size &&
            heap->data[left].dist < heap->data[smallest].dist)
            smallest = left;

        if (right < heap->size &&
            heap->data[right].dist < heap->data[smallest].dist)
            smallest = right;

        if (smallest == i)
            break;

        swap(&heap->data[i], &heap->data[smallest]);
        i = smallest;
    }
}

void push(MinHeap* heap, int node, CostUnit dist) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, sizeof(MinHeapNode) * heap->capacity);
    }

    int i = heap->size++;
    heap->data[i].node = node;
    heap->data[i].dist = dist;

    heapifyUp(heap, i);
}

MinHeapNode pop(MinHeap* heap) {
    MinHeapNode root = heap->data[0];

    heap->data[0] = heap->data[--heap->size];
    heapifyDown(heap, 0);

    return root;
}

CostUnit* dijkstra(Graph* graph, NodeId source, NodeId parent[]) {
    CostUnit* distances = malloc(sizeof(CostUnit) * graph->size);

    for (int i = 0; i < graph->size; i++) {
        distances[i] = INT_MAX;
        parent[i] = -1;
    }

    distances[source] = 0;

    MinHeap* heap = createHeap(graph->size);
    push(heap, source, 0);

    while (!isEmpty(heap)) {
        MinHeapNode top = pop(heap);
        int u = top.node;

        if (top.dist > distances[u]) continue;

        Edge* e = graph->adjacents[u];
        while (e) {
            int v = e->destination;
            CostUnit w = e->cost;

            if (distances[u] + w < distances[v]) {
                distances[v] = distances[u] + w;
                parent[v] = u;
                push(heap, v, distances[v]);
            }

            e = e->next;
        }
    }

    freeHeap(heap);

    return distances;
}