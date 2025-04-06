#!/bin/bash

URL="http://localhost:8080/search_flights"

TESTS=(
'{"origin": "SOF", "destination": "IST"}'
'{"origin": "SOF", "destination": "MLE"}'
'{"origin": "NYC", "destination": "SOF"}'
'{"origin": "SOF", "destination": "NYC"}'
'{"origin": "SOF", "destination": "NYC", "maxFlights": 2}'
'{"origin": "SOF", "destination": "NYC", "maxFlights": 0}'
'{"origin": "SOF", "destination": "FRA"}'
'{"origin": "IST", "destination": "NYC", "maxFlights": 3}'
'{"origin": "AUH", "destination": "SOF", "maxFlights": 5}'
'{"origin": "SOF", "destination": "SOF"}'
'{"origin": "SOF", "destination": "SOF", "maxFlights": 0}'
)

DESCRIPTIONS=(
"Expected: SOF -> IST, price 10"
"Expected: SOF -> IST -> CMB -> MLE, price 70"
"Expected: []"
"Expected: SOF -> FRA -> NYC, price 130
                  SOF -> LHR -> NYC, price 130
                  SOF -> FRA -> LHR -> NYC, price 135
                  SOF -> IST -> FRA -> NYC, price 145
                  SOF -> IST -> FRA -> LHR -> NYC, price 150"
"Expected: SOF -> FRA -> NYC, price 130
                  SOF -> LHR -> NYC, price 130"
"Expected: []"
"Expected: SOF -> FRA, price 30
                  SOF -> IST -> FRA, price 45"
"Expected: IST → FRA → NYC, price 135
                  IST → FRA → LHR → NYC, price 140"
"Expected: []"
"Expected: SOF, price 0"
"Expected: []"
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
