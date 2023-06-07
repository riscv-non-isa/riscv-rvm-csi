HEADER_SOURCE := header.adoc
PDF_RESULT := RVM-CSI.pdf
SSOT_YAML := api/rvm-csi-spec.yaml
DOCS_OUT_DIR := auto-gen
CODE_OUT_DIR := api/C/include

all: build

build:
	@echo "Auto-generating docs and code from YAML"
	python3 spec-schema/parser/csi_parser.py --generate-docs $(SSOT_YAML) --doc-out-dir=$(DOCS_OUT_DIR)
	python3 spec-schema/parser/csi_parser.py $(SSOT_YAML) --out-dir=$(CODE_OUT_DIR)

	@echo "Building PDF from asciidoc"
	asciidoctor-pdf \
    --attribute=mathematical-format=svg \
    --attribute=pdf-fontsdir=docs-resources/fonts \
    --attribute=pdf-style=docs-resources/themes/riscv-pdf.yml \
    --failure-level=ERROR \
    --require=asciidoctor-bibtex \
    --require=asciidoctor-diagram \
    --out-file=$(PDF_RESULT) \
    $(HEADER_SOURCE)

clean:
	rm $(PDF_RESULT)
	rm -rf $(DOCS_OUT_DIR)/*
