#!/bin/bash

URL="http://localhost:8080/search_flights"

TESTS=(
'{"origin": "SOF", "destination": "IST"}'
'{"origin": "SOF", "destination": "MLE"}'
'{"origin": "NYC", "destination": "SOF"}'
'{"origin": "SOF", "destination": "NYC"}'
'{"origin": "SOF", "destination": "NYC", "maxFlights": 2}'
'{"origin": "SOF", "destination": "FRA"}'
)

DESCRIPTIONS=(
"Expected: SOF → IST, price 10"
"Expected: SOF → IST → CMB → MLE, total price 70"
"Expected: No path available from NYC to SOF"
"Expected: Two paths to NYC: SOF→FRA→NYC (130), SOF→LHR→NYC (130)"
"Expected: Same two paths, both with 2 flights (<= maxFlights=2)"
"Expected: SOF→FRA (30) and SOF→IST→FRA (45); no loops allowed"
)

for i in "${!TESTS[@]}"; do
    echo "=============================="
    echo "Test $((i+1)): ${DESCRIPTIONS[$i]}"
    echo "Request:"
    echo "${TESTS[$i]}"
    echo "Sending request to $URL..."
    echo ""
    curl -s -X POST "$URL" \
         -H "Content-Type: application/json" \
         -d "${TESTS[$i]}"
    echo ""
    echo "=============================="
    echo ""
done
